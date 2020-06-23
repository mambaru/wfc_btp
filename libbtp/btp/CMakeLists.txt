
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_library(LIBRT rt)

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

add_dependencies(btpstorage rocksdb )

target_include_directories(btpstorage PUBLIC ${wfc_btp_SOURCE_DIR}/external/rocksdb)
target_include_directories(btpstorage PUBLIC ${wfc_btp_SOURCE_DIR}/external/rocksdb/include)
target_link_libraries(btpstorage PUBLIC wrtstat rocksdb ${Boost_LIBRARIES} ${LIBRT})
target_include_directories(btpstorage PUBLIC ${libbtp_SOURCE_DIR})

