cmake_minimum_required(VERSION 2.8.12)
PROJECT(noisyhunter)

set(EXECUTABLE_NAME "noisyhunter.bin")

set(myproject_VERSION_MAJOR 0)
set(myproject_VERSION_MINOR 1)
set(myproject_VERSION_PATCH 0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_definitions(-Wall -Wextra -Wshadow -g -pedantic -fPIC -std=c++11)
set(CMAKE_BUILD_TYPE "Debug")

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
#pkg_search_module(SDL2_gfx REQUIRED SDL2_gfx)
pkg_search_module(SDL2_image REQUIRED SDL2_image)
pkg_search_module(SDL2_mixer REQUIRED SDL2_mixer)
pkg_search_module(SDL2_ttf REQUIRED SDL2_ttf)
include_directories(${SDL2_INCLUDE_DIRS}
                    ${SDL2_gfx_INCLUDE_DIRS}
                    ${SDL2_ttf_INCLUDE_DIRS}
                    ${SDL2_image_INCLUDE_DIRS}
                    ${SDL2_mixer_INCLUDE_DIRS})

add_executable(${EXECUTABLE_NAME} "src/main.cpp")

set(helper
    "src/helper/renderHelper.cpp"
    "src/helper/renderHelper.hpp"
    "src/helper/errorHelper.cpp")

add_library(helper_lib
            ${helper})

target_link_libraries(${EXECUTABLE_NAME}
                        ${helper_lib}
                        ${SDL2_LIBRARIES}
                        ${SDL2_gfx_LIBRARIES}
                        ${SDL2_ttf_LIBRARIES}
                        ${SDL2_image_LIBRARIES}
                        ${SDL2_mixer_LIBRARIES}
                        )

