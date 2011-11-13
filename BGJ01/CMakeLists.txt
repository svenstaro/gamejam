project(cannonium)

cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(externals/ducttape-engine)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/ducttape-engine/cmake-extras/")

# Qt4 stuff
set(QT_USE_QTSCRIPT TRUE)
set(QT_DONT_USE_QTGUI TRUE)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITONS})

if(MSVC) # Sort the odd stuff out
    set(CMAKE_CXX_FLAGS "/DCOMPILER_MSVC /DOgreProcedural_STATIC /WX /MP /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG") # Odd duplicate symbols with /GL
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi")
else() # GCC/Mingw
    set(CMAKE_CXX_FLAGS "-DCOMPILER_GCC -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-but-set-parameter -pedantic -std=c++0x -fPIC")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2 -flto -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif()

get_directory_property(ducttape-includes DIRECTORY externals/ducttape-engine INCLUDE_DIRECTORIES)
include_directories(src/ ${ducttape-includes})

file(GLOB_RECURSE cannonium_SOURCES src/*.cpp)

file(GLOB_RECURSE cannonium_HEADERS src/*.hpp)
qt4_wrap_cpp(cannonium_HEADERS_MOC ${cannonium_HEADERS})

add_executable(cannonium ${cannonium_SOURCES} ${cannonium_HEADERS_MOC})

target_link_libraries(cannonium ducttape)
