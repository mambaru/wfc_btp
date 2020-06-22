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

  typedef wfc::json::enumerator<
    sort_by, 
    wfc::json::member_list<
      wfc::json::enum_value<n_empty, sort_by, sort_by::NOSORT>,
      wfc::json::enum_value<n_nosort, sort_by, sort_by::NOSORT>,
      wfc::json::enum_value<n_avg, sort_by, sort_by::AVG>,
      wfc::json::enum_value<n_count, sort_by, sort_by::COUNT>,
      wfc::json::enum_value<n_perc50, sort_by, sort_by::PERC50>,
      wfc::json::enum_value<n_perc80, sort_by, sort_by::PERC80>,
      wfc::json::enum_value<n_perc95, sort_by, sort_by::PERC95>,
      wfc::json::enum_value<n_perc99, sort_by, sort_by::PERC99>,
      wfc::json::enum_value<n_perc100, sort_by, sort_by::PERC100>,
      wfc::json::enum_value<n_min, sort_by, sort_by::MIN>,
      wfc::json::enum_value<n_max, sort_by, sort_by::MAX>,
      wfc::json::enum_value<n_lossy, sort_by, sort_by::LOSSY>
    >
  > type;

  typedef type::target target;
  typedef type::member_list member_list;
  typedef type::serializer serializer;
};

}}
