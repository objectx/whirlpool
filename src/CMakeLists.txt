
cmake_minimum_required (VERSION 3.14)

target_sources (${PROJECT_NAME} PRIVATE whirlpool.cpp ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.inc)

if (PYTHONINTERP_FOUND)
    add_custom_command (OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.inc
                        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/gen-table.py
                                ${CMAKE_CURRENT_SOURCE_DIR}/whirlpool.inc
                        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/gen-table.py
                        COMMENT Generate a Whirlpool constant table)
endif ()

target_compile_features (whirlpool PUBLIC cxx_std_14)
