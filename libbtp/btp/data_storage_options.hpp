#pragma once 

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

#ifndef CONFIG_PATH
#define CONFIG_PATH .
#endif


namespace wamba{ namespace btp{

struct data_storage_options
{
  size_t hash_size = 128;
  // Путь к базе данных для всех префиксов
  std::string db_path = "./data_storage";
  // Путь для WAL, если не указан то по умолчанью из rocksdb
  // если путь указано в ini, то wal_path + "prefix" + ini.wal_dir
  std::string wal_path = "";
  // Файл опций в формате ini
  std::string ini_path = STRINGIZE_VALUE_OF(CONFIG_PATH)"/data_storage.ini";
  
  time_t TTL = 0;
  size_t result_limit = 3000;
  bool create_if_missing = true;
  
  /*
  typedef std::function< void(const std::string&)> log_fun_t;
  log_fun_t log_fun;
  */
};

}}

