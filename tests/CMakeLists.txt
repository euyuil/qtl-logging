# Find Google Test
if(NOT TARGET GTest::gtest_main)
    find_package(GTest REQUIRED)
endif()

add_executable(test_logging
    test_logging_params.cpp
    test_sink_managers.cpp
    test_loggable.cpp
    test_logger_registry.cpp
)

target_link_libraries(test_logging
    PRIVATE
        qtl-logging
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_logging)
