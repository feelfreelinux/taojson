// Copyright (c) 2016-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAOCPP_JSON_INCLUDE_SAX_PARSE_FILE_HH
#define TAOCPP_JSON_INCLUDE_SAX_PARSE_FILE_HH

#include "../internal/grammar.hh"
#include "../internal/action.hh"
#include "../internal/control.hh"

#include "../external/pegtl/file_parser.hpp"

namespace tao
{
   namespace json
   {
      namespace sax
      {
         // SAX producer to parse a file containing a JSON string representation
         template< typename Consumer >
         void parse_file( const std::string & filename, Consumer & consumer )
         {
            json_pegtl::file_parser( filename ).parse< internal::grammar, internal::action, internal::control >( consumer );
         }

      } // sax

   } // json

} // tao

#endif
