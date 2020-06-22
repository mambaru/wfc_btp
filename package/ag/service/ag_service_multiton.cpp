//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//

#include "ag_service_multiton.hpp"
#include "ag_service.hpp"

#include <wfc/module/multiton.hpp>
#include <wfc/name.hpp>

namespace wamba{ namespace btp{ namespace ag{
  
namespace 
{
  WFC_NAME2(service_name, "ag-service")

  class impl
    : public ::wfc::jsonrpc::service_multiton< service_name, service::method_list> 
  {};
}

service_multiton::service_multiton()
  : wfc::component( std::make_shared<impl>() )
{
}

}}}
