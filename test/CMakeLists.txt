
cmake_minimum_required (VERSION 3.5)

include_directories (${WHIRLPOOL_SOURCE_DIR}/ext/Catch/include)

add_executable (test-whirlpool src/main.cxx)
target_link_libraries (test-whirlpool whirlpool)
target_compile_features (test-whirlpool PRIVATE cxx_auto_type cxx_lambda_init_captures)

add_test (NAME test-whirlpool
          COMMAND test-whirlpool)
