
cmake_minimum_required (VERSION 3.5)

find_package (Catch2 REQUIRED)

add_executable (test-whirlpool main.cpp)
target_link_libraries (test-whirlpool PRIVATE whirlpool Catch2::Catch2)
target_compile_features (test-whirlpool PRIVATE cxx_auto_type cxx_lambda_init_captures)

add_test (NAME test-whirlpool
          COMMAND test-whirlpool)
