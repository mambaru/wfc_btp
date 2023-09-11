add_executable(
  storage_test
    storage_suite.cpp
    key_cache_suite.cpp
    key_aggregator_suite.cpp
    key_storage_suite.cpp
    data_storage_suite.cpp
    storage_test.cpp
)

set_source_files_properties(
  key_storage_suite.cpp
  data_storage_suite.cpp
  PROPERTIES
  COMPILE_FLAGS "-Wno-sign-conversion"
)

add_dependencies(storage_test btpstorage)
target_link_libraries(storage_test PRIVATE btpstorage wrtstat fas::testing)
target_compile_definitions(storage_test PUBLIC "-DCONFIG_PATH=${wfc_btp_SOURCE_DIR}/tests")
wci_test(TARGET storage_test COMMAND storage_test NO_MEMCHECK)


