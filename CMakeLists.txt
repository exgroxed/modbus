cmake_minimum_required(VERSION 3.15)

project(modbus C)

set(CMAKE_C_STANDARD 99)

add_library(modbus STATIC src/modbus.c)
target_include_directories(modbus PUBLIC include)

add_executable(test_modbus test/test_modbus.c)
target_link_libraries(test_modbus PRIVATE modbus)