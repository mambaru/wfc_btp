//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2017
//
// Copyright: See COPYING file that comes with this distribution
//

#include "btp_build_info.h"
#include "btp_package.hpp"
#include "rt/rt_module.hpp"
#include "ag/ag_module.hpp"
#include <wfc/module/module_list.hpp>

namespace wamba{ namespace btp{

namespace
{ 
  class impl: public ::wfc::module_list<
    btp_build_info,
    rt_module,
    ag_module
  >
  {};
}

btp_package::btp_package()
  : package( std::make_shared<impl>() )
{
}

}}
