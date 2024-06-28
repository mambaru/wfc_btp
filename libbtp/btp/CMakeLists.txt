get_boost(system filesystem)
find_library(LIBRT rt)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(
  btpstorage
    storage.cpp
    key_aggregator.cpp
    key_cache.cpp
    key_storage.cpp
    key_rocksdb.cpp
    data_storage.cpp
    data_rocksdb.cpp
    shard_rocksdb.cpp
    data_storage.cpp
    aux.cpp
    merge_operator.cpp
)

wci_targets(btpstorage)

add_dependencies(btpstorage rocksdb )

target_link_libraries(btpstorage
  PUBLIC rocksdb
  PRIVATE wrtstat wlog Boost::system Boost::filesystem ${LIBRT} Threads::Threads)

target_include_directories(btpstorage PUBLIC $<BUILD_INTERFACE:${btpstorage_SOURCE_DIR}>
                                             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

#install(DIRECTORY btpstorage DESTINATION include/btpstorage FILES_MATCHING PATTERN "*.hpp" )
#install(TARGETS btpstorage LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
