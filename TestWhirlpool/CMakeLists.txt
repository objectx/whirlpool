
cmake_minimum_required (VERSION 3.0)

add_executable (TestWhirlpool src/main.cxx)
target_link_libraries (TestWhirlpool Whirlpool)

add_test (NAME TestWhirlpool
    COMMAND TestWhirlpool)
