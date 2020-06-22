#pragma once

#include <vector>
#include <string>
#include <array>
#include <sstream>

namespace wamba { namespace btp{ namespace rtdepr {

typedef int64_t counter_t;
typedef int64_t microtime_t;
    
typedef std::array<counter_t, 8> aggregated_flat;

struct aggregated
{
  counter_t avg = 0;
  counter_t count = 0;
  counter_t perc50 = 0;
  counter_t perc80 = 0;
  counter_t perc95 = 0;
  counter_t perc99 = 0;
  counter_t perc100 = 0;

  bool is_null() const
  {
    return avg == 0 &&
           count == 0 &&
           perc50 == 0 &&
           perc80 == 0 &&
           perc95 == 0 &&
           perc99 == 0 &&
           perc100 == 0;
  }

  inline aggregated_flat to_flat() const
  {
    return aggregated_flat{{0L, avg, count, perc50, perc80, perc95, perc99, perc100}};
  }
};

inline aggregated& operator += (aggregated& left, const aggregated& right)
{
  counter_t total = left.count + right.count;
  if (right.count==0)
    return left;
  left.avg    = ( left.avg*left.count    + right.avg*right.count   )/total;
  left.perc50 = ( left.perc50*left.count + right.perc50*right.count)/total;
  left.perc80 = ( left.perc80*left.count + right.perc80*right.count)/total;
  left.perc95 = ( left.perc95*left.count + right.perc95*right.count)/total;
  left.perc99 = ( left.perc99*left.count + right.perc99*right.count)/total;
  left.perc100 = ( left.perc100*left.count + right.perc100*right.count)/total;
  left.count = total;
  return left;
}

inline bool operator == (const aggregated& left, const aggregated& right)
{
  return left.avg == right.avg
    && left.count == right.count
    && left.perc50 == right.perc50
    && left.perc80 == right.perc80
    && left.perc95 == right.perc95
    && left.perc99 == right.perc99
    && left.perc100 == right.perc100;
}

typedef std::vector<aggregated_flat> aggregated_flat_list;

typedef std::vector<aggregated> aggregated_list;
typedef std::vector<counter_t>  counter_list;

struct name_ts
{
  std::string name;
  microtime_t ts = 0;
};

typedef std::pair<name_ts, counter_t> name_ts_counter_t;

typedef std::vector<name_ts_counter_t> name_ts_counters_t;

enum node_type
{
  BRANCH,
  LEAF,
  ALL
};

enum merge_type
{
  REPLACE,
  ADD,
  KEEP_DST
};

enum selector_type
{
  NOSORT,
  AVG,
  COUNT,
  PERC50,
  PERC80,
  PERC95,
  PERC99,
  PERC100
};

typedef std::vector<std::string> strvector_t;

}}}
