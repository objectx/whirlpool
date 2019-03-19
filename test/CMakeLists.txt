
cmake_minimum_required (VERSION 3.14)

find_package (Catch2 REQUIRED)
find_package (fmt REQUIRED)

add_executable (test-${PROJECT_NAME})

target_sources (test-${PROJECT_NAME} PRIVATE helper.cpp helper.hpp whirlpool.cpp main.cpp)
target_link_libraries (test-${PROJECT_NAME}
                       PRIVATE ${PROJECT_NAME} whirlpool Catch2::Catch2 fmt::fmt)
target_compile_features (test-${PROJECT_NAME} PRIVATE cxx_std_14)

if (COMMAND cotire)
    set_source_files_properties (main.cpp PROPERTIES COTIRE_EXCLUDED YES)
    cotire (test-${PROJECT_NAME})
endif ()

add_test (NAME test-${PROJECT_NAME}
          COMMAND test-${PROJECT_NAME})
