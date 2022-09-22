#pragma once

#include <btp/types.hpp>
#include <wfc/json.hpp>

namespace wamba { namespace btp {

struct sort_by_json
{
  JSON_NAME2(n_empty, "")
  JSON_NAME(nosort)
  JSON_NAME(avg)
  JSON_NAME(count)
  JSON_NAME(perc50)
  JSON_NAME(perc80)
  JSON_NAME(perc95)
  JSON_NAME(perc99)
  JSON_NAME(perc100)
  JSON_NAME(min)
  JSON_NAME(max)
  JSON_NAME(lossy)

  typedef wjson::enumerator<
    sort_by, 
    wjson::member_list<
      wjson::enum_value<n_empty, sort_by, sort_by::NOSORT>,
      wjson::enum_value<n_nosort, sort_by, sort_by::NOSORT>,
      wjson::enum_value<n_avg, sort_by, sort_by::AVG>,
      wjson::enum_value<n_count, sort_by, sort_by::COUNT>,
      wjson::enum_value<n_perc50, sort_by, sort_by::PERC50>,
      wjson::enum_value<n_perc80, sort_by, sort_by::PERC80>,
      wjson::enum_value<n_perc95, sort_by, sort_by::PERC95>,
      wjson::enum_value<n_perc99, sort_by, sort_by::PERC99>,
      wjson::enum_value<n_perc100, sort_by, sort_by::PERC100>,
      wjson::enum_value<n_min, sort_by, sort_by::MIN>,
      wjson::enum_value<n_max, sort_by, sort_by::MAX>,
      wjson::enum_value<n_lossy, sort_by, sort_by::LOSSY>
    >
  > type1;

  typedef type1::target target;
  typedef type1::member_list member_list;
  typedef type1::serializer serializer;
};

}}
