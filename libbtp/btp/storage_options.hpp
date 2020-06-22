#pragma once 

#include <btp/key_cache_options.hpp>
#include <btp/key_storage_options.hpp>
#include <btp/data_storage_options.hpp>

namespace wamba{ namespace btp{

struct storage_options
{
  key_cache_options key_cache;
  key_storage_options key_db;
  data_storage_options data_db;
  
  
  typedef std::function<void(std::string)> trace_fun_t;
  trace_fun_t trace;
};

}}

