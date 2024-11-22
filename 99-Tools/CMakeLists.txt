cmake_minimum_required(VERSION 3.5)
project(Tools C)

add_executable(KillProcess KillProcess.c)
target_include_directories(KillProcess PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CursorGen CursorGen.c)
target_include_directories(CursorGen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

