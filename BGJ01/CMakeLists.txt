project(cannonium)

cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(externals/ducttape-engine)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/ducttape-engine/cmake-extras/")
message(${CMAKE_MODULE_PATH})

get_directory_property(ducttape-includes DIRECTORY externals/ducttape-engine INCLUDE_DIRECTORIES)
include_directories(src/ ${ducttape-includes})

file(GLOB_RECURSE cannonium_SOURCES src/*.cpp)

file(GLOB_RECURSE cannonium_HEADERS src/*.hpp)
qt4_wrap_cpp(cannonium_HEADERS_MOC ${cannonium_HEADERS})

add_executable(cannonium ${cannonium_SOURCES} ${cannonium_HEADERS_MOD})

target_link_libraries(cannonium ducttape)
