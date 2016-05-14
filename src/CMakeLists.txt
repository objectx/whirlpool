
cmake_minimum_required (VERSION 3.5)

set (SOURCE_FILES Whirlpool.cxx)
set (HEADER_FILES ${WHIRLPOOL_SOURCE_DIR}/include/whirlpool.h)

add_library (whirlpool ${SOURCE_FILES} ${HEADER_FILES})
    target_include_directories (whirlpool PUBLIC $<BUILD_INTERFACE:${WHIRLPOOL_SOURCE_DIR}/include>
                                                 $<INSTALL_INTERFACE:include>)
