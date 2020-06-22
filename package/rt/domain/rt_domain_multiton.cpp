//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//


#include "rt_domain_multiton.hpp"
#include "rt_config_json.hpp"
#include "rt_domain.hpp"

#include <wfc/module/multiton.hpp>
#include <wfc/module/instance.hpp>
#include <wfc/name.hpp>

namespace wamba{ namespace btp{

namespace
{
  WFC_NAME2(object_name, "btp-rt")

  class impl: public ::wfc::multiton<
    object_name,
    wfc::instance<rt_domain>,
    rt_config_json,
    wfc::component_features::Defaults,
    wfc::defstat_json
  >
  {
    virtual std::string interface_name() const override
    {
      return "irt";
    }
  };
}

rt_domain_multiton::rt_domain_multiton()
  : component( std::make_shared<impl>() )
{
}

}}
