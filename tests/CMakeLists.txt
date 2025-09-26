add_executable(run_tests 
    test_logger.cpp
    test_md5.cpp
    test_csv_parser.cpp
    test_timer.cpp
    test_traversal.cpp
    test_threadsafe_queue.cpp
)
target_link_libraries(run_tests 
    PRIVATE 
    tests_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_tests)