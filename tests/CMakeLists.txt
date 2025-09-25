add_executable(run_tests 
    test_logger.cpp 
    test_md5.cpp    
)
target_link_libraries(run_tests 
    PRIVATE 
    tests_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)