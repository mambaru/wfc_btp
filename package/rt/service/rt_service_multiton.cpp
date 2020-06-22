//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//

#include "rt_service_multiton.hpp"
#include "rt_service.hpp"

#include <wfc/module/multiton.hpp>
#include <wfc/name.hpp>

namespace wamba{ namespace btp{
  
namespace 
{
  WFC_NAME2(service_name, "rt-service")

  class impl
    : public ::wfc::jsonrpc::service_multiton< service_name, service::method_list> 
  {};
}

rt_service_multiton::rt_service_multiton()
  : wfc::component( std::make_shared<impl>() )
{
}

}}
