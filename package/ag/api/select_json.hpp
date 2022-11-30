#pragma once
#include <ag/api/sort_by_json.hpp>
#include <ag/api/select.hpp>
#include <wfc/json.hpp>


namespace wamba { namespace btp{

namespace request {
  
  struct select_json
  {
    JSON_NAME(prefix)
    JSON_NAME(suffix)
    JSON_NAME(offset)
    JSON_NAME(limit)
    JSON_NAME(sortby)
    JSON_NAME(power)
    
    typedef wjson::object<
      select,
      wjson::member_list<
        wjson::member<n_prefix, select, std::string, &select::prefix>,
        wjson::member<n_suffix, select, std::string, &select::suffix>,
        wjson::member<n_offset, select, size_t,      &select::offset>,
        wjson::member<n_limit,  select, size_t,      &select::limit>,
        wjson::member<n_sortby, select, sort_by,     &select::by, sort_by_json>,
        wjson::member<n_power,  select, bool,        &select::power>
      >
    > meta;
    
    typedef meta::target target;
    typedef meta::member_list member_list;
    typedef meta::serializer serializer;
  };
}

namespace response
{
  struct select_json
  {
    JSON_NAME(names)
    
    typedef wjson::object_array<
      name_value_t,
      wjson::member_list<
        wjson::member_array< name_value_t, std::string, &name_value_t::first >,
        wjson::member_array< name_value_t, value_type, &name_value_t::second >
      >
    > name_value_json;
    
    typedef wjson::object<
      select,
      wjson::member_list<
        wjson::member<n_names, select, selected_names_t, &select::names, wjson::vector_of<name_value_json, 64> >
      >
    > meta;
    
    typedef meta::target target;
    typedef meta::serializer serializer;
    typedef meta::member_list member_list;
  };
}

}}
