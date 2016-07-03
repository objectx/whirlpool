
cmake_minimum_required (VERSION 3.5)

set (SOURCE_FILES whirlpool.cpp)
set (HEADER_FILES ${WHIRLPOOL_SOURCE_DIR}/include/whirlpool.hpp whirlpool.inc)

if (${PYTHONINTERP_FOUND})
    add_custom_command (OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.inc
                        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/gen-table.py
                                ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.inc
                        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/gen-table.py
                        COMMENT Generate a Whirlpool constant table)
endif ()

add_library (whirlpool ${SOURCE_FILES} ${HEADER_FILES})
    target_include_directories (whirlpool PUBLIC $<BUILD_INTERFACE:${WHIRLPOOL_SOURCE_DIR}/include>
                                                 $<INSTALL_INTERFACE:include>)
    target_compile_features (whirlpool PUBLIC cxx_auto_type cxx_alias_templates)
