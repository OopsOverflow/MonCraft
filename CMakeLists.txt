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

message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "Build Target : ${BUILD_TARGET}")
message(STATUS "Build Platform : ${BUILD_PLATFORM}")

set(BUILDPATH ${CMAKE_BINARY_DIR})

if(${BUILD_PLATFORM} STREQUAL wasm)
    set(BUILDPATH "${BUILDPATH}/Wasm")

else()
    set(BUILDPATH "${BUILDPATH}/App")

endif()

if(${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(BUILDPATH "${BUILDPATH}/Release")
    
else()
    set(BUILDPATH "${BUILDPATH}/Debug")

endif()

if(${BUILD_TARGET} STREQUAL client)
    set(BUILDPATH "${BUILDPATH}/Client")
    add_definitions(-DMONCRAFT_CLIENT)
    add_definitions(-DGLEW_STATIC)
    
else()
    set(BUILDPATH "${BUILDPATH}/Server")
    add_definitions(-DMONCRAFT_SERVER)

endif()

# Create build directories
file(MAKE_DIRECTORY ${BUILDPATH})

set(${PROJECT_BINARY_DIR} ${BUILDPATH})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Dependencies")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/OpenGL/src")

add_subdirectory(OpenGL)
