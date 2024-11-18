cmake_minimum_required(VERSION 3.5)
project(Tools C)

add_executable(KillProcess KillProcess.c)
target_include_directories(KillProcess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
