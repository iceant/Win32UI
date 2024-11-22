cmake_minimum_required(VERSION 3.5)
project(KeyboardMouse C)

add_executable(${PROJECT_NAME} WIN32 "")

########################################################################################################################
## SOURCES
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCES ${SOURCE_DIR}/*.c)
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})

