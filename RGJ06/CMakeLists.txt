cmake_minimum_required(VERSION 2.8.0)

project(RGJ06)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-extras/")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(SFML 2 COMPONENTS audio graphics system window REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
#find_package(ImageMagick COMPONENTS Magick++ REQUIRED)

#add_definitions(-Wall -Wextra -O2 -pedantic -fPIC -std=c++0x -DBOOST_FILESYSTEM_VERSION=3)
#add_definitions(-std=c++0x -DBOOST_FILESYSTEM_VERSION=3)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3 -pedantic -fPIC -std=c++0x -DBOOST_FILESYSTEM_VERSION=3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -pedantic -fPIC -std=c++0x -DBOOST_FILESYSTEM_VERSION=3")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(
	"${CMAKE_SOURCE_DIR}/src/"
	${Boost_INCLUDE_DIRS}
	${SFML_INCLUDE_DIR}
	#	${ImageMagick_INCLUDE_DIRS}
)

aux_source_directory(src/ sources)

include_directories(src/)

add_executable(energymaster
    ${sources}
)

target_link_libraries(energymaster
    ${Boost_LIBRARIES}
	${SFML_LIBRARIES}
	#${ImageMagick_LIBRARIES}
)
