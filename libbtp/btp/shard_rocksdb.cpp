#include <btp/shard_rocksdb.hpp>
#include <btp/data_rocksdb.hpp>
#include <rocksdb/env.h>
#include "aux.hpp"
#include "merge_operator.hpp"

namespace wamba{ namespace btp{

  class data_comparator
    : public rocksdb::Comparator
  {
  public:
    data_comparator() noexcept
      : rocksdb::Comparator() {}

    int Compare(const rocksdb::Slice& a, const rocksdb::Slice& b) const
    {
      const key_ts_t &aa = *reinterpret_cast<const key_ts_t*>(a.data());
      const key_ts_t &bb = *reinterpret_cast<const key_ts_t*>(b.data());

      return aa == bb ? 0 : aa < bb ? -1 : 1;
    }

    const char* Name() const { return "key_ts_t comparator"; }
    void FindShortestSeparator(std::string*, const ::rocksdb::Slice&) const { }
    void FindShortSuccessor(std::string*) const { }
  };

shard_rocksdb::shard_rocksdb()
{
}

shard_rocksdb::rocksdb_ptr shard_rocksdb::get_db(key_id_t key_id) const
{
  return _dbs[ key_id % _dbs.size() ];
}

shard_rocksdb::~shard_rocksdb()
{
  this->close(nullptr);
}

bool shard_rocksdb::close(std::string* err)
{
  if ( _dbs.empty() )
    return false;

  for (auto db : _dbs)
  {
    if ( db!=nullptr && !db->close(err) )
      return false;
  }
  _dbs.clear();
  _comparator = nullptr;
  return true;
}

bool shard_rocksdb::open(const data_storage_options& opt, std::string* err)
{
  if (!mkpath(opt.db_path, err) )
    return false;

  _cdf = {CFD()};

  auto status = ::rocksdb::LoadOptionsFromFile( _conf_opt, opt.ini_path, &_options, &_cdf );
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP data storage LoadOptionsFromFile error: ") + status.ToString();
    return false;
  }

  _options.env = _conf_opt.env;
  _options.create_if_missing = opt.create_if_missing;
  _comparator = std::make_shared<data_comparator>();
  _cdf[0].options.merge_operator = std::make_shared<merge_operator>();
  _cdf[0].options.comparator = _comparator.get();
  /*_comparator = std::make_shared<data_comparator>();
  _options.comparator = _comparator.get();
  */
  _dbs.resize( opt.hash_size == 0 ? 1 : opt.hash_size );

  for (size_t i = 0 ; i < _dbs.size(); ++i)
  {
    std::string path = opt.db_path + "/" + std::to_string(i);
    ::rocksdb::DBWithTTL* db;
    data_rocksdb::handles_list_t handles;
    status = ::rocksdb::DBWithTTL::Open(_options, path, _cdf, &handles, &db, std::vector<int32_t>{int32_t(opt.TTL) } );
    if ( !status.ok() )
    {
      if ( err != nullptr )
        *err = std::string("BTP data storage open error '") + path + "': " + status.ToString();
      return false;
    }

    _dbs[i] = std::make_shared<data_rocksdb>();
    _dbs[i]->open(db, handles, opt.result_limit);
  }
  return true;
}

bool shard_rocksdb::set(key_id_t id, const aggregated_info& data, std::string* err)
{
  return this->get_db(id)->set(id, data, err);
}

bool shard_rocksdb::inc(key_id_t id, const aggregated_info& data, std::string* err)
{
  return this->get_db(id)->inc(id, data, err);
}

bool shard_rocksdb::get(key_id_t id, aggregated_list* result, std::string* err, time_type ts, size_t offset, size_t limit)
{
  return this->get_db(id)->get(id, result, err, ts, offset, limit);
}

bool shard_rocksdb::compact(std::string* err)
{
  for (auto db : _dbs)
    if ( !db->compact(err) )
      return false;
  return true;
}

bool shard_rocksdb::del(key_id_t id, std::string* err)
{
  return this->get_db(id)->del(id, err);
}

}}

