
cmake_minimum_required (VERSION 3.14)

project (whirlpool
         VERSION 1.0.0
         LANGUAGES CXX)

list (INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/CMake ${CMAKE_BINARY_DIR})

function (setvar_default var_)
    if (NOT DEFINED ${var_})
        set (${var_} ${ARGN} PARENT_SCOPE)
    endif ()
endfunction ()

setvar_default (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
setvar_default (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
setvar_default (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

enable_testing ()

include (cotire OPTIONAL)

add_library (${PROJECT_NAME} STATIC)
target_include_directories (${PROJECT_NAME}
                            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                                   $<INSTALL_INTERFACE:include>)

add_subdirectory (src)
add_subdirectory (test)
