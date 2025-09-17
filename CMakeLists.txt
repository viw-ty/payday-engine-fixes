cmake_minimum_required(VERSION 3.31.7)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

add_compile_options(/W4 /Z7 /std:c++20)
add_link_options(/WHOLEARCHIVE:sblt_plugin.lib)

project(pd2mod)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

file(GLOB sources "src/*.cpp")

set(BUILD_SHARED_LIBS on)
add_library(pd2mod ${sources})

target_include_directories(pd2mod PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_directories(pd2mod PUBLIC "${CMAKE_SOURCE_DIR}/lib")

target_link_libraries(pd2mod sblt_plugin)
target_link_libraries(pd2mod minhook)
target_link_libraries(pd2mod EOSSDK-Win32-Shipping)
