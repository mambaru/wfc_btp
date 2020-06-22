#pragma once
#include <btp/types.hpp>
#include <functional>
#include <string>
#include <vector>
#include <memory>


namespace wamba{ namespace btp{

namespace request
{
  struct multi_get
  {
    bool power = false;
    time_type ts = 0;
    size_t offset = 0;
    size_t limit = 0;
    std::vector<std::string> names;
    typedef std::unique_ptr<multi_get> ptr;
  };
}

namespace response
{
  typedef std::pair<std::string, aggregated_list> data_get;

  struct multi_get 
  {
    std::vector<data_get> data;
    
    typedef std::unique_ptr<multi_get> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}
