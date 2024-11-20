cmake_minimum_required(VERSION 3.5)
project(GDIPlus_LinearGradientBrush C)

add_executable(${PROJECT_NAME} main.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

########################################################################################################################
##
SET(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCES ${SOURCE_DIR}/*.c)
list(FILTER SOURCES EXCLUDE REGEX main.c)
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC Gdiplus)