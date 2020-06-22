#pragma once
#include <btp/types.hpp>
#include <functional>
#include <string>
#include <memory>


namespace wamba{ namespace btp{

namespace request
{
  struct compact
  {
    typedef std::unique_ptr<compact> ptr;
  };
}

namespace response
{
  struct compact
  {
    typedef std::unique_ptr<compact> ptr;
    typedef std::function<void(ptr)> handler;
  };
}

}}
