cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) #remove RelWithDebInfo and MinSizeRel

# set the project name
set(PROJECT_NAME "MonCraft")
project(${PROJECT_NAME} VERSION 1.1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()


set(BUILD_TARGET client CACHE STRING "Choose build target client/server" )
set(BUILD_PLATFORM app CACHE STRING "Choose targer platform wasm/app" )

message(STATUS "Build Target : ${BUILD_TARGET}")
message(STATUS "Build Platform : ${BUILD_PLATFORM}")

set(BUILDPATH ${CMAKE_BINARY_DIR})

if(${BUILD_TARGET} STREQUAL client)
    set(BUILDPATH "${BUILDPATH}/Client")
    add_definitions(-DMONCRAFT_CLIENT)
    add_definitions(-DGLEW_STATIC)
    
    if(${BUILD_PLATFORM} STREQUAL wasm)
        set(BUILDPATH "${BUILDPATH}/Wasm")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
            "-g0 -O3
            -fno-rtti
            -Wno-error
            -Wno-pthreads-mem-growth
            -s WASM=1
            -pthread -s PTHREAD_POOL_SIZE=5
            -s USE_SDL=2
            -s USE_SDL_IMAGE=2
            -s SDL2_IMAGE_FORMATS='[\"png\"]'
            -s USE_SDL_GFX=2
            -s USE_FREETYPE=2
            -s ALLOW_MEMORY_GROWTH=1
            -s MAX_WEBGL_VERSION=2
            -s MIN_WEBGL_VERSION=2
            -s FULL_ES2=1
            -s FULL_ES3=1
            -s WEBSOCKET_URL='wss://pi.thissma.fr:55001'
            -s WEBSOCKET_SUBPROTOCOL='binary'
            --preload-file data --preload-file src -Wno-unused-command-line-argument")
            # MESON_ARGS = --prefix=${PWD}/WASM --cross-file=emscripten-crossfile.txt --default-library=static --buildtype=debug --wrap-mode=nofallback

    else()
        set(BUILDPATH "${BUILDPATH}/App")

    endif()

else()
    set(BUILDPATH "${BUILDPATH}/Server")
    add_definitions(-DMONCRAFT_SERVER)

endif()

# Create build directories
file(MAKE_DIRECTORY ${BUILDPATH})
set(DEFAULT_BUILD_TYPE "Release")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILDPATH}/Release")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILDPATH}/Debug")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILDPATH}/Release")
message(STATUS "Written in ${BUILDPATH}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Dependencies")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/OpenGL/src")

add_subdirectory(OpenGL)
