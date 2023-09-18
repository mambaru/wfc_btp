#pragma once

#include <btp/data_storage.hpp>

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <rocksdb/utilities/options_util.h>
#include <rocksdb/utilities/db_ttl.h>

namespace wamba{ namespace btp{

typedef std::pair<key_id_t, time_type> key_ts_t;

class data_rocksdb
{
  typedef ::rocksdb::ColumnFamilyDescriptor CFD;
  typedef std::vector<CFD> CFD_list;
  typedef ::rocksdb::DBWithTTL db_t;
public:
  typedef std::vector< ::rocksdb::ColumnFamilyHandle*> handles_list_t;
  virtual ~data_rocksdb();

  bool open(db_t* db, const handles_list_t& handles, size_t result_limit);

  bool close(std::string* err);

  bool set(key_id_t id, const aggregated_info& data, std::string* err);

  bool get(key_id_t id, aggregated_list* result, std::string* err, time_type ts, size_t offset, size_t limit);

  bool del(key_id_t id, std::string* err);

  bool compact(std::string* err);

  bool inc(key_id_t id, const aggregated_info& data, std::string* err);

  db_t* get_db() { return _db; }
private:
  size_t _result_limit = 3000;
  ::rocksdb::ReadOptions _ro;
  ::rocksdb::WriteOptions _wo;
  db_t* _db;
  handles_list_t _handles;

};

}}

