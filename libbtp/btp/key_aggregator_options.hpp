#pragma once 

#include <wrtstat/aggregator/options/aggregator_options.hpp>

namespace wamba{ namespace btp{

struct key_aggregator_options:
  wrtstat::aggregator_options
{
  // Аггрегация последней точки
  bool aggregate_last_point = false;
};

}}

