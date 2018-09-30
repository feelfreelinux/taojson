// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_LIST_TRAITS_HPP
#define TAO_JSON_CONTRIB_LIST_TRAITS_HPP

#include <list>

#include <tao/json/consume.hpp>
#include <tao/json/forward.hpp>

#include "internal/array_traits.hpp"

namespace tao
{
   namespace json
   {
      template< typename T, typename... Ts >
      struct list_traits
         : public internal::array_traits< std::list< T, Ts... > >
      {
         template< template< typename... > class Traits, typename Base >
         static void to( const basic_value< Traits, Base >& v, std::list< T, Ts... >& r )
         {
            const auto& a = v.get_array();
            for( const auto& i : a ) {
               r.emplace_back( i.template as< T >() );
            }
         }

         template< template< typename... > class Traits, typename Producer >
         static void consume( Producer& parser, std::list< T, Ts... >& r )
         {
            auto s = parser.begin_array();
            while( parser.element_or_end_array( s ) ) {
               r.emplace_back( json::consume< T, Traits >( parser ) );
            }
         }
      };

   }  // namespace json

}  // namespace tao

#endif
