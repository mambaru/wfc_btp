#pragma once

#include <rt/api/depr/add.hpp>
#include <wfc/json.hpp>

namespace wamba { namespace btp{ namespace rtdepr {

namespace request {
  
  typedef wfc::json::array<std::vector<wfc::json::value<counter_t> > > counter_list_json;
  
  struct add_json
  {
    JSON_NAME(ts)
    JSON_NAME(name)
    JSON_NAME(cl)
    JSON_NAME(count)
    
    typedef wfc::json::object<
      add,
      fas::type_list_n<
        wfc::json::member<n_ts, add, microtime_t, &add::ts>,
        wfc::json::member<n_name, add, std::string, &add::name>,
        wfc::json::member<n_cl, add, counter_list, &add::cl, counter_list_json>,
        wfc::json::member<n_count, add, long, &add::count>
      >::type
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct add_json
  {
    FAS_NAME(result)
    
    typedef wfc::json::object<
      add,
      fas::type_list_n<
        wfc::json::member<n_result, add, bool, &add::result>
      >::type
    > type;

    typedef type::target target;
    typedef type::serializer serializer;
  };
}

}}}
