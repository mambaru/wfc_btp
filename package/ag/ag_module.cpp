//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//

#include "ag_module.hpp"
#include "deprecated/btp_deprecated_gateway_multiton.hpp"
#include "domain/ag_domain_multiton.hpp"
#include "service/ag_service_multiton.hpp"
#include "gateway/ag_gateway_multiton.hpp"



#include <wfc/module/component_list.hpp>
#include <wfc/name.hpp>

namespace wamba{ namespace btp{

namespace
{
  WFC_NAME2(module_name, "ag-module")

  class impl: public ::wfc::component_list<
    module_name,
    ag_domain_multiton,
    ag::service_multiton,
    ag::gateway_multiton,
    btp_deprecated_gateway_multiton
  >
  {
  public:
  };
}

ag_module::ag_module()
  : module( std::make_shared<impl>() )
{
}

}}
