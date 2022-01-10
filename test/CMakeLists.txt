
cmake_minimum_required (VERSION 3.14)

find_package (Catch2 REQUIRED)
find_package (fmt REQUIRED)

add_executable (test-${PROJECT_NAME})

target_sources (test-${PROJECT_NAME} PRIVATE helper.cpp helper.hpp whirlpool.cpp main.cpp)
target_link_libraries (test-${PROJECT_NAME}
                       PRIVATE ${PROJECT_NAME} ${PROJECT_NAME}-settings_ whirlpool Catch2::Catch2 fmt::fmt)
target_compile_features (test-${PROJECT_NAME} PRIVATE cxx_std_14)

if (WHIRLPOOL_ENABLE_PRECOMPILED_HEADERS)
    target_precompile_headers (test-${PROJECT_NAME}
                               PRIVATE <fmt/format.h>
                                       <catch2/catch.hpp>
                               PRIVATE <iostream>
                                       <iomanip>
                                       <string>)
endif ()

set_source_files_properties (main.cpp PROPERTIES SKIP_PRECOMPILE_HEADERS YES)

add_test (NAME test-${PROJECT_NAME}
          COMMAND test-${PROJECT_NAME})
