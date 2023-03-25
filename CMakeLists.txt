
cmake_minimum_required (VERSION 3.20)

project (whirlpool
         VERSION 1.0.0
         LANGUAGES CXX)

option (WHIRLPOOL_ENABLE_TESTS "Enable tests" YES)
option (WHIRLPOOL_ENABLE_SANITIZERS "Enable sanitizers" NO)
option (WHIRLPOOL_ENABLE_PRECOMPILED_HEADERS "Enable precompiled headers" NO)

function (setvar_default var_)
    if (NOT DEFINED "${var_}")
        set ("${var_}" "${ARGN}" PARENT_SCOPE)
    endif ()
endfunction ()

setvar_default (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
setvar_default (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
setvar_default (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if (WHIRLPOOL_ENABLE_TESTS)
    enable_testing ()
endif ()

set (itf_ "${PROJECT_NAME}-settings_")
add_library ("${itf_}" INTERFACE)
if (WHIRLPOOL_ENABLE_SANITIZERS)
    target_compile_options ("${itf_}" INTERFACE "-fsanitize=address,undefined")
    target_link_options ("${itf_}" INTERFACE "-fsanitize=address,undefined")
endif ()

add_library ("${PROJECT_NAME}" STATIC)
target_include_directories ("${PROJECT_NAME}"
                            PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
                                   "$<INSTALL_INTERFACE:include>")
target_link_libraries ("${PROJECT_NAME}" PRIVATE "${PROJECT_NAME}-settings_")

add_subdirectory (src)

if (WHIRLPOOL_ENABLE_TESTS)
    add_subdirectory (test)
endif ()
