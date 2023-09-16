//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//

#pragma once
#include <vector>
#include <string>
#include <btp/storage_options.hpp>

namespace wamba{ namespace btp{

struct ag_config: storage_options
{
  bool btp_trace = true;
  /* Если демон сконфигурирован с несколькими инстансами, то загрузка БД осуществляется паралельно */
  bool mtload = false;
};

}}
