
cmake_minimum_required (VERSION 3.5)

set (SOURCE_FILES whirlpool.cpp)
set (HEADER_FILES ${WHIRLPOOL_SOURCE_DIR}/include/whirlpool.hpp)

add_library (whirlpool ${SOURCE_FILES} ${HEADER_FILES})
    target_include_directories (whirlpool PUBLIC $<BUILD_INTERFACE:${WHIRLPOOL_SOURCE_DIR}/include>
                                                 $<INSTALL_INTERFACE:include>)
    target_compile_features (whirlpool PUBLIC cxx_auto_type cxx_alias_templates)
