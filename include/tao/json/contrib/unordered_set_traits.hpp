// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_UNORDERED_SET_TRAITS_HPP
#define TAO_JSON_CONTRIB_UNORDERED_SET_TRAITS_HPP

#include <unordered_set>

#include <tao/json/consume.hpp>
#include <tao/json/forward.hpp>

#include "internal/array_traits.hpp"

namespace tao
{
   namespace json
   {
      template< typename T, typename... Ts >
      struct unordered_set_traits
         : public internal::array_traits< std::unordered_set< T, Ts... > >
      {
         template< template< typename... > class Traits, typename Base >
         static void to( const basic_value< Traits, Base >& v, std::unordered_set< T, Ts... >& r )
         {
            const auto& a = v.get_array();
            for( const auto& i : a ) {
               r.emplace( i.template as< T >() );
            }
         }

         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& parser, std::unordered_set< T, Ts... >& r )
         {
            auto s = parser.begin_array();
            while( parser.element_or_end_array( s ) ) {
               r.emplace( json::consume< T, Traits >( parser ) );
            }
         }
      };

   }  // namespace json

}  // namespace tao

#endif
