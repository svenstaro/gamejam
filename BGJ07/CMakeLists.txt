cmake_minimum_required(VERSION 2.8.12)
PROJECT(noisyhunter)

set(noisyhunter_VERSION_MAJOR 0)
set(noisyhunter_VERSION_MINOR 1)
set(noisyhunter_VERSION_PATCH 0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_definitions(-Wall -Wextra -Wshadow -g -pedantic -fPIC -std=c++11)
set(CMAKE_BUILD_TYPE "Debug")

find_package(Bullet REQUIRED)

find_program(SDL2_CONFIG sdl2-config)
execute_process(COMMAND ${SDL2_CONFIG} "--cflags" OUTPUT_VARIABLE SDL_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND ${SDL2_CONFIG} "--libs" OUTPUT_VARIABLE SDL_LIBRARIES OUTPUT_STRIP_TRAILING_WHITESPACE)

add_definitions(${SDL_CFLAGS})

aux_source_directory(src/ srcs)

include_directories(SYSTEM ${BULLET_INCLUDE_DIRS})

add_executable(noisyhunter ${srcs})

target_link_libraries(noisyhunter ${SDL_LIBRARIES} -lSDL2_image -lSDL2_ttf ${BULLET_LIBRARIES})

