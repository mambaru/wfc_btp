#pragma once

#include <btp/key_storage_options.hpp>
#include <btp/stored_key.hpp>
#include <rocksdb/options.h>
#include <rocksdb/utilities/options_util.h>
#include <rocksdb/utilities/db_ttl.h>

namespace wamba{ namespace btp{

class key_rocksdb
{
public:
  typedef std::function<void( std::string, stored_key )> load_fun_t;

  key_rocksdb();
  virtual ~key_rocksdb();

  bool open(const key_storage_options& opt, std::string* err);

  bool close(std::string* err);

  bool set(const std::string& key, const stored_key& data, std::string* err);

  bool get(const std::string& key, stored_key* data, std::string* err);

  bool compact(std::string* err);

  bool load(load_fun_t fun, std::string* err );

  bool del(const std::string& key, std::string* err);

private:

  typedef ::rocksdb::ColumnFamilyDescriptor CFD;
  typedef std::vector<CFD> CFD_list;
  typedef ::rocksdb::DBWithTTL db_t;

  CFD_list _cdf;

  ::rocksdb::Env* _env;
  ::rocksdb::Options _options;
  ::rocksdb::WriteOptions _wo;
  ::rocksdb::ReadOptions _ro;

  db_t* _db;
};

}}

