// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_BINDING_HPP
#define TAOCPP_JSON_INCLUDE_BINDING_HPP

#include <string>
#include <type_traits>

#include "events/produce.hpp"
#include "external/pegtl/internal/pegtl_string.hpp"
#include "value.hpp"

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif

namespace tao
{
   namespace json
   {
      namespace binding
      {
         template< typename T, T, typename = void >
         struct element;

         template< typename C, typename T, T C::*P >
         struct element< T C::*, P, typename std::enable_if< std::is_member_object_pointer< T C::* >::value >::type >
         {
            static auto read( const C& v ) -> decltype( v.*P )
            {
               return v.*P;
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, read( v ) );
            }
         };

         template< typename C, typename T, T C::*P >
         struct element< T C::*, P, typename std::enable_if< std::is_member_function_pointer< T C::* >::value >::type >
         {
            static auto read( const C& v ) -> decltype( ( v.*P )() )
            {
               return ( v.*P )();
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce( Consumer& consumer, const C& v )
            {
               events::produce< Traits >( consumer, read( v ) );
            }
         };

         template< char... Cs >
         using key = json_pegtl::string< Cs... >;

         template< typename K, typename T, T >
         struct member;

         template< char... Cs, typename T, T P >
         struct member< key< Cs... >, T, P >
            : element< T, P >
         {
            static std::string key()
            {
               const char s[] = { Cs..., 0 };
               return std::string( s, sizeof...( Cs ) );
            }

            template< template< typename... > class Traits = traits, typename Consumer >
            static void produce_key( Consumer& consumer )
            {
               static const char s[] = { Cs..., 0 };
               consumer.key( tao::string_view( s, sizeof...( Cs ) ) );
            }
         };

         template< typename... As >
         struct array
         {
            template< typename A, template< typename... > class Traits, typename Base, typename C >
            static bool unsafe_emplace_back( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace_back( A::read( x ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void assign( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace_array();
               using swallow = bool[];
               (void)swallow{ unsafe_emplace_back< As >( v, x )... };
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& x )
            {
               consumer.begin_array( sizeof...( As ) );
               using swallow = bool[];
               (void)swallow{ ( As::template produce< Traits >( consumer, x ), consumer.element(), true )... };
               consumer.end_array( sizeof...( As ) );
            }
         };

         template< typename... As >
         struct object
         {
            template< typename A, template< typename... > class Traits, typename Base, typename C >
            static bool unsafe_emplace( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace( A::key(), A::read( x ) );
               return true;
            }

            template< template< typename... > class Traits, typename Base, typename C >
            static void assign( basic_value< Traits, Base >& v, const C& x )
            {
               v.unsafe_emplace_object();
               using swallow = bool[];
               (void)swallow{ unsafe_emplace< As >( v, x )... };
            }

            template< template< typename... > class Traits = traits, typename Consumer, typename C >
            static void produce( Consumer& consumer, const C& x )
            {
               consumer.begin_object( sizeof...( As ) );
               using swallow = bool[];
               (void)swallow{ ( As::template produce_key< Traits >( consumer ), As::template produce< Traits >( consumer, x ), consumer.member(), true )... };
               consumer.end_object( sizeof...( As ) );
            }
         };

      }  // namespace binding

   }  // namespace json

}  // namespace tao

#if defined( __GNUC__ ) && ( __GNUC__ >= 7 ) && ( __cplusplus < 201703L )
#pragma GCC diagnostic pop
#endif

#define TAOCPP_JSON_BIND_ELEMENT( ... ) tao::json::binding::element< decltype( __VA_ARGS__ ), __VA_ARGS__ >
#define TAOCPP_JSON_BIND_MEMBER( KeY, ... ) tao::json::binding::member< TAOCPP_JSON_PEGTL_INTERNAL_STRING( tao::json::binding::key, KeY ), decltype( __VA_ARGS__ ), __VA_ARGS__ >

#endif
