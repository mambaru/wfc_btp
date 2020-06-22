#pragma once

#include <rt/api/depr/multi_add.hpp>
#include <wfc/json.hpp>
#include "add_json.hpp"

namespace wamba { namespace btp{ namespace rtdepr {

namespace request {
  
  namespace wj = ::wfc::json;
  
  struct multi_add_json
  {
    FAS_NAME(data)
  
    typedef wj::array<std::vector<add_json>> data_json;
    
    typedef wfc::json::object<
      multi_add,
      fas::type_list_n<
        wfc::json::member<n_data, multi_add, std::vector<add>, &multi_add::data, data_json>
      >::type
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct multi_add_json
  {
    FAS_NAME(result)
    
    typedef wfc::json::object<
      multi_add,
      fas::type_list_n<
        wfc::json::member<n_result, multi_add, bool, &multi_add::result>
      >::type
    > type;

    typedef type::target target;
    typedef type::serializer serializer;
  };
}

}}}
