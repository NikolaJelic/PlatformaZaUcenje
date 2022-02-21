cmake_minimum_required(VERSION 3.19)

project (platforma-za-ucenje)



set(CMAKE_BUILD_TYPE Debug CACHE STRING "")
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
message(STATUS "Exporting compile_commands.json")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()


# exe
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.?pp")
file(GLOB_RECURSE UTILS CONFIGURE_DEPENDS "util/*.?pp")

add_executable(${PROJECT_NAME} ${SOURCES} ${UTILS})
target_include_directories(${PROJECT_NAME} PRIVATE src include util)

target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
