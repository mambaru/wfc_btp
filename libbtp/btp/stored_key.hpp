#pragma once 

#include <btp/types.hpp>

namespace wamba{ namespace btp{

struct stored_pair
{
  value_type value = 0;
  time_type ts = 0;
};
  
struct stored_basic
{
  stored_pair avg;
  stored_pair perc50;
  stored_pair perc80;
  stored_pair perc95;
  stored_pair perc99;
  stored_pair perc100;
  stored_pair min;
  stored_pair max;
};
  
struct stored_values
{
  time_type last_update = 0;
  stored_pair count;
  stored_pair lossy;
  stored_basic max;
  stored_basic pow;
};

struct stored_key: stored_values
{
  key_id_t id = 0;
};

struct stored_name: stored_key
{
  std::string name;
};


}}

