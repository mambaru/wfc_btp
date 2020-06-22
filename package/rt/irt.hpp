#pragma once

#include <wfc/statistics/istatistics.hpp>

namespace wamba{ namespace btp{

struct irt
  : public wfc::istatistics
{
  virtual ~irt() {}
};

}}
