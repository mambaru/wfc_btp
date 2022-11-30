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
    > meta;
    
    typedef meta::target target;
    typedef meta::serializer serializer;
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
    > meta;
    
    typedef meta::target target;
    typedef meta::serializer serializer;
  };
}

}}
