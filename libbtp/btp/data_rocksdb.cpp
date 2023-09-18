#include <btp/data_rocksdb.hpp>
#include <limits>
#include <iostream>

namespace wamba{ namespace btp{

data_rocksdb::~data_rocksdb()
{
  this->close(nullptr);
}

bool data_rocksdb::close(std::string* )
{
  if ( _db == nullptr )
    return false;

  for ( auto& handle: _handles)
    _db->DestroyColumnFamilyHandle(handle);
  delete _db;
  _db = nullptr;
  return true;
}

bool data_rocksdb::open(db_t* db, const handles_list_t& handles, size_t result_limit)
{
  _result_limit = result_limit;
  _db = db;
  _handles = handles;
  return true;
}

bool data_rocksdb::set(key_id_t id, const aggregated_info& data, std::string* err)
{
  key_ts_t name(id, data.ts);
  ::rocksdb::Slice skey(reinterpret_cast<const char*>(&name), sizeof(key_ts_t));
  ::rocksdb::Slice svalue(reinterpret_cast<const char*>(&data), sizeof(aggregated_info));
  auto status = _db->Put(_wo, skey, svalue);
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP data storage Put error id='") + std::to_string(id) + "', ts=" + std::to_string(data.ts);
  }
  return status.ok();
}

bool data_rocksdb::inc(key_id_t id, const aggregated_info& data, std::string* err)
{
  key_ts_t name(id, data.ts);
  ::rocksdb::Slice skey(reinterpret_cast<const char*>(&name), sizeof(key_ts_t));
  ::rocksdb::Slice svalue(reinterpret_cast<const char*>(&data), sizeof(aggregated_info));
  auto status = _db->Merge(_wo, skey, svalue);
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP data storage Put error id='") + std::to_string(id) + "', ts=" + std::to_string(data.ts);
  }
  return status.ok();
}


bool data_rocksdb::get(key_id_t id, aggregated_list* result, std::string* err, time_type ts, size_t offset, size_t limit)
{
  if ( limit==0 || limit > _result_limit )
    limit = _result_limit;

  typedef ::rocksdb::Iterator iterator_type;
  typedef std::shared_ptr<iterator_type> iterator_ptr;
  result->clear();
  iterator_ptr itr( _db->NewIterator(_ro) );
  if ( itr == nullptr)
  {
    if ( err!=nullptr)
      *err = std::string("BTP data storage get error: bad iterator");
    return false;
  }
  result->reserve(_result_limit);
  key_ts_t key(id, ts);
  ::rocksdb::Slice skey(reinterpret_cast<const char*>(&key), sizeof(key_ts_t));
  itr->Seek(skey);
  while ( itr->Valid() )
  {
    const key_ts_t* pkey = reinterpret_cast<const key_ts_t*>( reinterpret_cast<const void*>(itr->key().data()) );
    if ( pkey==nullptr || pkey->first != id )
      break;
    if ( offset == 0 )
    {
      if ( limit != 0)
      {
        --limit;
        if ( const aggregated_info* pinfo = reinterpret_cast<const aggregated_info*>( reinterpret_cast<const void*>(itr->value().data()) ) )
          result->push_back( *pinfo );
      }

      if ( limit == 0)
        break;
    }
    else
      offset--;
    itr->Next();
  }
  if ( result->size() > _result_limit )
  {
    std::move(result->end() - _result_limit, result->end(), result->begin() );
    result->resize(_result_limit);
  }
  return true;
}

bool data_rocksdb::del(key_id_t id, std::string* err)
{
  key_ts_t key_from(id, 0);
  ::rocksdb::Slice skey_from(reinterpret_cast<const char*>(&key_from), sizeof(key_ts_t));

  typedef std::shared_ptr<rocksdb::Iterator> iterator_ptr;
  iterator_ptr itr(_db->NewIterator(_ro));
  
  itr->Seek(skey_from);
  while( itr->Valid() )
  {
    key_ts_t cur_k = *reinterpret_cast<const key_ts_t*>( reinterpret_cast<const void*>( itr->key().data() ) );
    if ( cur_k.first != id )
      break;
    auto status = _db->Delete(_wo, itr->key() );
    if ( !status.ok() )
    {
      if ( err != nullptr )
        *err = std::string("BTP data_rocksdb::del DeleteRange error :") + status.ToString();
      return false;
    }
    itr->Next();
  }

  return true;
}


bool data_rocksdb::compact(std::string* err)
{
  ::rocksdb::CompactRangeOptions cro;
  auto status = _db->CompactRange(cro, nullptr, nullptr);
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP data_rocksdb::compact CompactRange error :") + status.ToString();
  }
  return status.ok();
}

}}

