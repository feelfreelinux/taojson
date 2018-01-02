// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_EVENTS_NON_FINITE_TO_EXCEPTION_HPP
#define TAOCPP_JSON_INCLUDE_EVENTS_NON_FINITE_TO_EXCEPTION_HPP

#include <cmath>
#include <stdexcept>

namespace tao
{
   namespace json
   {
      namespace events
      {
         template< typename Consumer >
         struct non_finite_to_exception
            : public Consumer
         {
            using Consumer::Consumer;

            using Consumer::number;

            void number( const double v )
            {
               if( !std::isfinite( v ) ) {
                  throw std::runtime_error( "invalid non-finite double value" );  // NOLINT
               }
               Consumer::number( v );
            }
         };

      }  // namespace events

   }  // namespace json

}  // namespace tao

#endif
