#pragma once
#include <ag/api/compact.hpp>
#include <wfc/json.hpp>


namespace wamba { namespace btp {

namespace request {
  
  struct compact_json
  {
    typedef wjson::object<
      compact,
      wjson::member_list<
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::serializer serializer;
  };
}

namespace response
{
  struct compact_json
  {
    typedef wjson::object<
      compact,
      wjson::member_list<
      >
    > type1;
    
    typedef type1::target target;
    typedef type1::serializer serializer;
  };
}

}}
