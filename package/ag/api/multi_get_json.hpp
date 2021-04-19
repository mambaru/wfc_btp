#pragma once
#include <wrtstat/aggregator/api/json/aggregated_info_json_compact.hpp>
#include <ag/api/multi_get.hpp>
#include <wjson/json.hpp>



namespace wamba{ namespace btp{

namespace request {
  
  struct multi_get_json
  {
    JSON_NAME(ts)
    JSON_NAME(offset)
    JSON_NAME(limit)
    JSON_NAME(power)
    JSON_NAME(names)


    typedef wjson::object<
      multi_get,
      wjson::member_list<
        wjson::member<n_ts, multi_get, time_type, &multi_get::ts>,
        wjson::member<n_offset, multi_get, size_t, &multi_get::offset>,
        wjson::member<n_limit, multi_get, size_t, &multi_get::limit>,
        wjson::member<n_power, multi_get, bool, &multi_get::power>,
        wjson::member<n_names, multi_get, std::vector<std::string>, &multi_get::names, wjson::vector_of_strings<> >
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

namespace response
{
  struct multi_get_json
  {
    JSON_NAME(name)
    JSON_NAME(counters)

    typedef wjson::object<
      data_get,
      wjson::member_list<
        wjson::member<n_name, data_get, std::string, &data_get::first>,
        wjson::member<n_counters, data_get, aggregated_list, &data_get::second, 
          wjson::vector_of<wrtstat::aggregated_info_json_compact, 3000> >
      >
    > data_get_json;

    typedef wjson::vector_of<data_get_json, 16> data_get_list_json;

    //JSON_NAME(scale)
    JSON_NAME(data)
    

    typedef wjson::object<
      multi_get,
      wjson::member_list<
        //wjson::member<n_scale, multi_get, time_type, &multi_get::scale>,
        wjson::member<n_data,
          multi_get, std::vector<data_get>, &multi_get::data, data_get_list_json>
      >
    > type;
    
    typedef type::target target;
    typedef type::serializer serializer;
  };
}

}}
