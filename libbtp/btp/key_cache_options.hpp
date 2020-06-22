#pragma once 

#include <btp/key_aggregator_options.hpp>

namespace wamba{ namespace btp{

struct key_cache_options
  : key_aggregator_options
{
  time_t TTL = 0;
  // Интервал сборки мусора (устаревших ключей)
  time_t gc_interval = 0;
  size_t mutex_hash = 1024;
};

}}

