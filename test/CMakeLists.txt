
cmake_minimum_required (VERSION 3.20)

if (POLICY CMP0110)
    cmake_policy (SET CMP0110 NEW)
endif ()

find_package (Catch2 REQUIRED)
find_package (fmt REQUIRED)

add_executable ("test-${PROJECT_NAME}")

target_sources ("test-${PROJECT_NAME}" PRIVATE helper.cpp helper.hpp whirlpool.cpp)

target_link_libraries ("test-${PROJECT_NAME}"
                       PRIVATE "${PROJECT_NAME}" "${PROJECT_NAME}-settings_" Catch2::Catch2WithMain fmt::fmt)
target_compile_features ("test-${PROJECT_NAME}" PRIVATE cxx_std_14)

if (WHIRLPOOL_ENABLE_PRECOMPILED_HEADERS)
    target_precompile_headers ("test-${PROJECT_NAME}"
                               PRIVATE <fmt/format.h>
                                       <catch2/catch_test_macros.hpp>
                               PRIVATE <iostream>
                                       <iomanip>
                                       <string>)
endif ()

add_test (NAME "Test ${PROJECT_NAME}"
          COMMAND "test-${PROJECT_NAME}")
