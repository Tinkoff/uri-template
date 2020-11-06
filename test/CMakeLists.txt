cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_MACOSX_RPATH 1)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

add_executable(basic basic.cpp)
add_executable(parsing parsing.cpp)
add_executable(matching matching.cpp)

target_link_libraries(basic uri-template::uri-template gtest_main)
target_link_libraries(parsing uri-template::uri-template gtest_main)
target_link_libraries(matching uri-template::uri-template gtest_main)

add_test(NAME basic COMMAND basic)
add_test(NAME parsing COMMAND parsing)
add_test(NAME matching COMMAND matching)
