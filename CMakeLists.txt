cmake_minimum_required(VERSION 3.15)
project(qtl-logging VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(QTL_LOGGING_BUILD_TESTS "Build unit tests" ON)
option(QTL_LOGGING_BUILD_EXAMPLES "Build examples" ON)

# Dependencies
include(FetchContent)

# spdlog
find_package(spdlog QUIET)
if(NOT spdlog_FOUND)
    message(STATUS "spdlog not found, fetching from source...")
    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.13.0
    )
    FetchContent_MakeAvailable(spdlog)

    # Install spdlog when it's fetched so it can be exported
    install(TARGETS spdlog
        EXPORT qtl-logging-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
    )
endif()

# Library target
add_library(qtl-logging
    src/logging_params.cpp
    src/console_sink_manager.cpp
    src/file_sink_manager.cpp
    src/loggable.cpp
    src/logger_registry.cpp
)

target_include_directories(qtl-logging
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(qtl-logging
    PUBLIC
        spdlog::spdlog
)

# Tests
if(QTL_LOGGING_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(QTL_LOGGING_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Install
install(TARGETS qtl-logging
    EXPORT qtl-logging-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT qtl-logging-targets
    FILE qtl-logging-targets.cmake
    NAMESPACE qtl::
    DESTINATION lib/cmake/qtl-logging
)

# Create package config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/qtl-logging-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/qtl-logging-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/qtl-logging-config.cmake"
    INSTALL_DESTINATION lib/cmake/qtl-logging
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/qtl-logging-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/qtl-logging-config-version.cmake"
    DESTINATION lib/cmake/qtl-logging
)

message(STATUS "")
message(STATUS "qtl-logging Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Tests: ${QTL_LOGGING_BUILD_TESTS}")
message(STATUS "  Build Examples: ${QTL_LOGGING_BUILD_EXAMPLES}")
message(STATUS "")
