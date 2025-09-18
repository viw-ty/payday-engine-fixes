cmake_minimum_required(VERSION 3.31.7)

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

add_compile_options(/W4 /Z7 /std:c++20)
add_link_options(/WHOLEARCHIVE:sblt_plugin.lib)

project(engine-fixes)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

file(GLOB sources "src/*.cpp")

set(BUILD_SHARED_LIBS on)
add_library(engine-fixes ${sources})

target_include_directories(engine-fixes PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_link_directories(engine-fixes PUBLIC "${CMAKE_SOURCE_DIR}/lib")

target_link_libraries(engine-fixes sblt_plugin)
target_link_libraries(engine-fixes minhook)
target_link_libraries(engine-fixes EOSSDK-Win32-Shipping)
