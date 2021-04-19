//
// Author: Vladimir Migashko <migashko@gmail.com>, (C) 2013-2015
//
// Copyright: See COPYING file that comes with this distribution
//
#pragma once

#include "ag_config.hpp"
#include <btp/types.hpp>
#include <wfc/json.hpp>
#include <wrtstat/aggregator/options/aggregator_options_json.hpp>



namespace wamba{ namespace btp{

struct key_cache_json
{
  JSON_NAME(mutex_hash)
  JSON_NAME(aggregate_last_point)
  JSON_NAME(gc_interval)
  JSON_NAME(TTL)
  
  typedef wfc::json::object<
    key_cache_options,
    wfc::json::member_list<
      wfc::json::member<n_mutex_hash, key_cache_options, size_t, &key_cache_options::mutex_hash>,
      wfc::json::member<n_aggregate_last_point, key_aggregator_options, bool, &key_aggregator_options::aggregate_last_point>,
      wfc::json::member<n_TTL, key_cache_options, time_type, &key_cache_options::TTL>,
      wfc::json::member<n_gc_interval, key_cache_options, time_type, &key_cache_options::gc_interval>,
      wfc::json::base<wrtstat::aggregator_options_json>
    >
  > type;

  typedef type::serializer serializer;
  typedef type::target target;
  typedef type::member_list member_list;
};

struct key_storage_json
{
  JSON_NAME(db_path)
  JSON_NAME(wal_path)
  JSON_NAME(ini_path)
  JSON_NAME(auto_repair)
  JSON_NAME(TTL)
  JSON_NAME(create_if_missing)

  typedef wfc::json::object<
    key_storage_options,
    wfc::json::member_list<
      wfc::json::member<n_db_path, key_storage_options, std::string, &key_storage_options::db_path>,
      wfc::json::member<n_wal_path, key_storage_options, std::string, &key_storage_options::wal_path>,
      wfc::json::member<n_ini_path, key_storage_options, std::string, &key_storage_options::ini_path>,
      wfc::json::member<n_TTL, key_storage_options, time_type, &key_storage_options::TTL>,
      wfc::json::member<n_create_if_missing, key_storage_options, bool, &key_storage_options::create_if_missing>,
      wfc::json::member<n_auto_repair, key_storage_options, bool, &key_storage_options::auto_repair>
    >
  > type;

  typedef type::serializer serializer;
  typedef type::target target;
  typedef type::member_list member_list;
};

struct data_storage_json
{
  JSON_NAME(hash_size)
  JSON_NAME(db_path)
  JSON_NAME(wal_path)
  JSON_NAME(ini_path)
  JSON_NAME(TTL)
  JSON_NAME(result_limit)
  JSON_NAME(create_if_missing)

  typedef wfc::json::object<
    data_storage_options,
    wfc::json::member_list<
      wfc::json::member<n_db_path,      data_storage_options, std::string, &data_storage_options::db_path>,
      wfc::json::member<n_wal_path,     data_storage_options, std::string, &data_storage_options::wal_path>,
      wfc::json::member<n_ini_path,     data_storage_options, std::string, &data_storage_options::ini_path>,
      wfc::json::member<n_TTL,          data_storage_options, time_type,   &data_storage_options::TTL>,
      wfc::json::member<n_result_limit, data_storage_options, size_t,      &data_storage_options::result_limit>,
      wfc::json::member<n_hash_size,    data_storage_options, size_t,      &data_storage_options::hash_size>,
      wfc::json::member<n_create_if_missing, data_storage_options, bool,   &data_storage_options::create_if_missing>
    >
  > type;

  typedef type::serializer serializer;
  typedef type::target target;
  typedef type::member_list member_list;
};

struct ag_config_json
{
  JSON_NAME(key_cache)
  JSON_NAME(key_db)
  JSON_NAME(data_db)
  
  typedef wfc::json::object<
    ag_config,
    wfc::json::member_list<
      wfc::json::member<n_key_cache, storage_options, key_cache_options,    &storage_options::key_cache, key_cache_json>,
      wfc::json::member<n_key_db,    storage_options, key_storage_options,  &storage_options::key_db,    key_storage_json>,
      wfc::json::member<n_data_db,   storage_options, data_storage_options, &storage_options::data_db,   data_storage_json>
    >
  > type;

  typedef type::serializer serializer;
  typedef type::target target;
  typedef type::member_list member_list;
};

}}
