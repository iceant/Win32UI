cmake_minimum_required(VERSION 3.5)
project(ResourceDemo C)

add_executable(${PROJECT_NAME} WIN32 "")

########################################################################################################################
## SOURCES
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCES ${SOURCE_DIR}/*.c)
file(GLOB RESOURCE_SOURCES ${SOURCE_DIR}/*.rc)
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})
target_sources(${PROJECT_NAME} PUBLIC ${RESOURCE_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})

