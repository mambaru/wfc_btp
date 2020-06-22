//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//
#pragma once

#include "rt_config.hpp"
#include <wfc/json.hpp>
#include <wfc/statistics/stat_options_json.hpp>

namespace wamba{ namespace btp{

struct rt_config_json
{
  JSON_NAME(targets)
  
  typedef wfc::json::object<
    rt_config,
    wfc::json::member_list<
      wfc::json::base<wrtstat::agmap_options_json>,
      wfc::json::member< n_targets,    rt_config, std::vector<std::string>, &rt_config::targets, wfc::json::vector_of_strings<> >
    >,
    wfc::json::strict_mode
  > type;

  typedef type::serializer serializer;
  typedef type::target target;
  typedef type::member_list member_list;
};

}}
