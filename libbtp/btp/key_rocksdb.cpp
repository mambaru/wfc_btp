#include <btp/key_rocksdb.hpp>
#include "aux.hpp"

namespace wamba{ namespace btp{

key_rocksdb::key_rocksdb()
  : _env(nullptr)
{
  
}
key_rocksdb::~key_rocksdb()
{
  this->close(nullptr);
}

bool key_rocksdb::close(std::string* )
{
  if ( _db == nullptr )
    return false;
  delete _db;
  _db = nullptr;
  return true;
}

bool key_rocksdb::open(const key_storage_options& opt, std::string* err)
{
  if (!mkpath(opt.db_path, err) )
    return false;
  
  _env = ::rocksdb::Env::Default();
  auto status = ::rocksdb::LoadOptionsFromFile( opt.ini_path, _env, &_options, &_cdf );
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP key storage LoadOptionsFromFile error: ") + status.ToString();
    return false;
  }
  _options.env = _env;
  _options.create_if_missing = opt.create_if_missing;

  status = ::rocksdb::DBWithTTL::Open(_options, opt.db_path, &_db, opt.TTL );
    
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP key storage open error '") + opt.db_path + "': " + status.ToString();
    return false;
  }
   
  return true;
}
  
bool key_rocksdb::set(const std::string& key, const stored_key& value, std::string* err)
{
  auto status = _db->Put(_wo, key, rocksdb::Slice(reinterpret_cast<const char*>(&value), sizeof(value)));
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP key storage Put error '") + key+ ": " + status.ToString();
  }
  return status.ok();
}
  
bool key_rocksdb::get(const std::string& key, /*std::string* value*/stored_key* data, std::string* err)
{
  std::string value;
  auto status = _db->Get(_ro, key, &value);
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP key storage Get error key='") + key+ "': " + status.ToString();
  }
  else
  {
    *data  = *(reinterpret_cast<const stored_key*>( value.data() ));
  }
  return status.ok();
}
  
bool key_rocksdb::compact(std::string* err)
{
  ::rocksdb::CompactRangeOptions cro;
  auto status = _db->CompactRange(cro, nullptr, nullptr);
  if ( !status.ok() )
  {
    if ( err != nullptr )
      *err = std::string("BTP key storage CompactRange error :") + status.ToString();
  }
  return status.ok();
}
  
bool key_rocksdb::load(load_fun_t fun, std::string* err )
{
  typedef ::rocksdb::Iterator iterator_type;
  typedef std::shared_ptr<iterator_type> iterator_ptr;
  
  iterator_ptr itr( _db->NewIterator(_ro) );
  if ( itr == nullptr)
  {
    if ( err!=nullptr)
      *err = std::string("BTP key storage load error: bad iterator");
    return false;
  }

  itr->SeekToFirst();
  while ( itr->Valid() ) 
  {
    const stored_key* sk = reinterpret_cast<const stored_key*>( itr->value().data() );
    fun( itr->key().ToString(), *sk );
    itr->Next(); 
  }
  return true;
}

}}

