//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//


#include "ag_domain_multiton.hpp"
#include "ag_config_json.hpp"
#include "ag_domain.hpp"

#include <wfc/module/multiton.hpp>
#include <wfc/module/instance.hpp>
#include <wfc/name.hpp>

namespace wamba{ namespace btp{

namespace
{
  WFC_NAME2(object_name, "btp-ag")

  class impl: public ::wfc::multiton<
    object_name,
    wfc::instance<ag_domain>,
    ag_config_json,
    wfc::component_features::Defaults,
    wfc::defstat_json
  >
  {
    virtual std::string interface_name() const override
    {
      return "iag";
    }
  };
}

ag_domain_multiton::ag_domain_multiton()
  : component( std::make_shared<impl>() )
{
}

}}
