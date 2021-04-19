#pragma once

#include <wrtstat/api/push_json.hpp>
#include <wrtstat/api/multi_push_json.hpp>
#include <wrtstat/api/del_json.hpp>
#include <ag/iag.hpp>
#include <ag/api/get_json.hpp>
#include <ag/api/merge_json.hpp>
#include <ag/api/multi_get_json.hpp>
#include <ag/api/tree_json.hpp>
#include <ag/api/select_json.hpp>
#include <ag/api/compact_json.hpp>
#include <ag/api/get_names_json.hpp>
#include <wfc/jsonrpc.hpp>

namespace wamba{ namespace btp{ namespace ag{ namespace service{

  JSONRPC_TAG(push)
  JSONRPC_TAG(multi_push)
  JSONRPC_TAG(del)
  
JSONRPC_TAG(get)
JSONRPC_TAG(merge)
JSONRPC_TAG(multi_get)
JSONRPC_TAG(tree)
JSONRPC_TAG(get_name_tree)
JSONRPC_TAG(select)
JSONRPC_TAG(get_names)
JSONRPC_TAG(compact)

struct method_list: wfc::jsonrpc::method_list
<
  wfc::jsonrpc::target< ibtp >,
  wfc::jsonrpc::invoke_method< _push_, ::wrtstat::request::push_json, ::wrtstat::response::push_json, wfc::istatistics, &wfc::istatistics::push>,
  wfc::jsonrpc::invoke_method< _multi_push_, ::wrtstat::request::multi_push_json, ::wrtstat::response::multi_push_json, wfc::istatistics, &wfc::istatistics::multi_push>,
  wfc::jsonrpc::invoke_method< _del_, ::wrtstat::request::del_json, ::wrtstat::response::del_json, wfc::istatistics, &wfc::istatistics::del>,
  
  wfc::jsonrpc::invoke_method<  _get_,    request::get_json,    response::get_json,    ibtp, &ibtp::get >,
  wfc::jsonrpc::invoke_method<  _merge_,    request::merge_json,    response::merge_json,    ibtp, &ibtp::merge >,
  wfc::jsonrpc::invoke_method<  _multi_get_,    request::multi_get_json,    response::multi_get_json,    ibtp, &ibtp::multi_get >,
  wfc::jsonrpc::invoke_method<  _tree_,   request::tree_json,   response::tree_json,   ibtp, &ibtp::tree >,
  wfc::jsonrpc::invoke_method<  _get_name_tree_,   request::get_name_tree_json,   response::get_name_tree_json,   ibtp, &ibtp::get_name_tree >,
  wfc::jsonrpc::invoke_method<  _select_, request::select_json, response::select_json, ibtp, &ibtp::select >,
  wfc::jsonrpc::invoke_method<  _get_names_, request::get_names_json, response::get_names_json, ibtp, &ibtp::get_names >,
  wfc::jsonrpc::invoke_method<  _compact_, request::compact_json, response::compact_json, ibtp, &ibtp::compact >
>
{};

}}}}
