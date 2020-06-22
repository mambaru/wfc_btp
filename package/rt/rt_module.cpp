//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//

#include "rt_module.hpp"
#include "domain/rt_domain_multiton.hpp"
#include "service/rt_service_multiton.hpp"
/*#include "gateway/rt_gateway_multiton.hpp"
*/
#include <wfc/module/component_list.hpp>
#include <wfc/name.hpp>

namespace wamba{ namespace btp{

namespace
{
  WFC_NAME2(module_name, "rt-module")

  class impl: public ::wfc::component_list<
    module_name,
    rt_domain_multiton,
    rt_service_multiton
  >
  {
  public:
    virtual std::string description() const override
    {
      return "BTP Real-time aggregation module";
    }
  };
}

rt_module::rt_module()
  : module( std::make_shared<impl>() )
{
}

}}
