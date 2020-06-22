#pragma once
#include <ag/api/compact.hpp>
#include <wfc/json.hpp>


namespace wamba { namespace btp {

namespace request {
  
  struct compact_json
  {
    typedef wfc::json::object<
      compact,
      wfc::json::member_list<
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct compact_json
  {
    typedef wfc::json::object<
      compact,
      wfc::json::member_list<
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

}}
