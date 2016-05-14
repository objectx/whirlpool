
cmake_minimum_required (VERSION 3.5)

add_library (Whirlpool Whirlpool.cxx)
target_include_directories (Whirlpool PUBLIC $<BUILD_INTERFACE:${WHIRLPOOL_SOURCE_DIR}/include>
                                             $<INSTALL_INTERFACE:include>)
