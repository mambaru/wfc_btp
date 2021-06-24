#pragma once

#include <btp/data_storage.hpp>

#include <rocksdb/db.h>
#include <rocksdb/env.h>
#include <rocksdb/options.h>
#include <rocksdb/utilities/options_util.h>
#include <rocksdb/utilities/db_ttl.h>

namespace wamba{ namespace btp{

class data_rocksdb;
class data_comparator;

class shard_rocksdb
{
  typedef ::rocksdb::ColumnFamilyDescriptor CFD;
  typedef std::vector<CFD> CFD_list;
  typedef ::rocksdb::DBWithTTL db_t;
  typedef std::shared_ptr<data_rocksdb> rocksdb_ptr;

public:
  shard_rocksdb();

  virtual ~shard_rocksdb();

  bool open(const data_storage_options& opt, std::string* err);

  bool close(std::string* err);

  bool set(key_id_t key_id, const aggregated_info& data, std::string* err);

  bool get(key_id_t id, aggregated_list* result, std::string* err, time_type ts, size_t offset, size_t limit);

  bool del(key_id_t id, std::string* err);

  bool compact(std::string* err);

  rocksdb_ptr get_db(key_id_t key_id) const;


private:
  ::rocksdb::Env* _env;
  ::rocksdb::Options _options;
  CFD_list _cdf;
  std::vector<rocksdb_ptr> _dbs;
  std::shared_ptr<data_comparator> _comparator;
};

}}

