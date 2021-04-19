add_executable(test_btp test_btp.cpp btp_suite.cpp)
target_link_libraries(test_btp PUBLIC wfc_btp rocksdb fas::testing)
add_dependencies(test_btp wfc_btp)
wci_test(TARGET test_btp COMMAND test_btp)
