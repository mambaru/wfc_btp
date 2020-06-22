//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//

#pragma once
#include <vector>
#include <string>
#include <wrtstat/wrtstat_options.hpp>

namespace wamba{ namespace btp{

struct rt_config: public wrtstat::agmap_options
{
  std::vector<std::string> targets;
};

}}
