
find_package(Boost COMPONENTS system filesystem REQUIRED)
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
)

wci_targets(btpstorage)

add_dependencies(btpstorage rocksdb )

target_include_directories(btpstorage PUBLIC ${wfc_btp_SOURCE_DIR}/external/rocksdb)
target_include_directories(btpstorage PUBLIC ${wfc_btp_SOURCE_DIR}/external/rocksdb/include)
target_link_libraries(btpstorage  PRIVATE  rocksdb wrtstat wlog  ${Boost_LIBRARIES} ${LIBRT} Threads::Threads)
target_include_directories(btpstorage PUBLIC ${libbtp_SOURCE_DIR})

