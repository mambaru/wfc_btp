#pragma once 

#include <mutex>
#include <wrtstat/aggregated_data.hpp>

namespace wamba{ namespace btp{

typedef uint64_t key_id_t;
typedef ::wrtstat::aggregated_data aggregated_data;
typedef ::wrtstat::aggregated_info aggregated_info;
typedef std::vector<aggregated_info> aggregated_list;
typedef ::wrtstat::value_type value_type;
typedef ::wrtstat::time_type  time_type; 
typedef std::pair< std::string, value_type> name_value_t;
typedef std::vector< name_value_t > selected_names_t;

  
typedef std::mutex mutex_type;
constexpr key_id_t bad_key = static_cast<key_id_t>(-1);

enum class sort_by
{
  NOSORT,
  AVG,
  COUNT,
  PERC50,
  PERC80,
  PERC95,
  PERC99,
  PERC100,
  MIN,
  MAX,
  LOSSY
  // ,
  // POWER_AVG, /* avg * count */
  // POWER80    /* PERC80 * count */
};

enum class tree_node
{
  ANY,
  LEAF,
  BRANCH
};


}}

