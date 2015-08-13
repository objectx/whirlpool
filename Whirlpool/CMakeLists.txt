
cmake_minimum_required (VERSION 3.0)

add_library (Whirlpool src/Whirlpool.cxx)
target_include_directories (Whirlpool PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/Whirlpool>
    )
