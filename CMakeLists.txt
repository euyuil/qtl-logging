cmake_minimum_required(VERSION 3.15)
project(qtl-logging VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# A parent project owns the build tree's global CTest setup. CMake 3.15 does
# not provide PROJECT_IS_TOP_LEVEL, so compare the source roots instead.
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(QTL_LOGGING_IS_TOP_LEVEL ON)
else()
    set(QTL_LOGGING_IS_TOP_LEVEL OFF)
endif()

if(QTL_LOGGING_IS_TOP_LEVEL)
    include(CTest)
endif()

# Options
option(
    QTL_LOGGING_BUILD_TESTS
    "Build qtl-logging unit tests"
    ${QTL_LOGGING_IS_TOP_LEVEL}
)
option(
    QTL_LOGGING_BUILD_EXAMPLES
    "Build qtl-logging examples"
    ${QTL_LOGGING_IS_TOP_LEVEL}
)

# Dependencies
find_package(spdlog REQUIRED)

# Library target
add_library(qtl-logging
    src/logging_params.cpp
    src/console_sink_manager.cpp
    src/file_sink_manager.cpp
    src/loggable.cpp
    src/logger_registry.cpp
)

# Alias for consistent namespaced target name in both build and install trees
add_library(qtl::qtl-logging ALIAS qtl-logging)

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
set(QTL_LOGGING_TESTS_ENABLED OFF)
if(BUILD_TESTING AND QTL_LOGGING_BUILD_TESTS)
    set(QTL_LOGGING_TESTS_ENABLED ON)
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
message(STATUS "  Build Tests: ${QTL_LOGGING_TESTS_ENABLED}")
message(STATUS "  Build Examples: ${QTL_LOGGING_BUILD_EXAMPLES}")
message(STATUS "")
