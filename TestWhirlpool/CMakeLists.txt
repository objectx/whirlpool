
cmake_minimum_required (VERSION 3.0)

include_directories ("${CMAKE_SOURCE_DIR}/Catch/include")

add_executable (TestWhirlpool src/main.cxx)
target_link_libraries (TestWhirlpool Whirlpool)
target_compile_features (TestWhirlpool PRIVATE cxx_auto_type cxx_lambda_init_captures)

add_test (NAME TestWhirlpool
    COMMAND TestWhirlpool)
