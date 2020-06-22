//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//

#include "ag_gateway_multiton.hpp"
#include "ag_gateway.hpp"

#include <wfc/module/multiton.hpp>
#include <wfc/module/instance.hpp>
#include <wfc/name.hpp>

namespace wamba{ namespace btp{ namespace ag{

namespace {

  WFC_NAME2(component_name, "ag-gateway")

  class impl
    : public ::wfc::jsonrpc::gateway_multiton< 
        component_name, 
        ag::gateway::method_list, 
        ag::gateway::interface_
    > 
  {
  public:
    virtual std::string interface_name() const override
    {
      return std::string("wfc::btp::iag");
    }

    virtual std::string description() const override
    {
      return "Gateway for BTP system";
    }
  };
}

gateway_multiton::gateway_multiton()
  : wfc::component( std::make_shared<impl>() )
{
}

}}}
