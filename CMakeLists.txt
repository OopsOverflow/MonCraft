cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE) #remove RelWithDebInfo and MinSizeRel

if (WIN32)
    include(${CMAKE_SOURCE_DIR}/Dependencies/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif ()

# set the project name
set(PROJECT_NAME "MonCraft")
project(${PROJECT_NAME} VERSION 1.1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(BUILD_TARGET client CACHE STRING "Choose build target client/server" )
set(BUILD_PLATFORM app CACHE STRING "Choose targer platform wasm/app" )
set(ENABLE_SANITIZERS OFF CACHE BOOL "Enable ASan / UBSan sanitizers (in debug mode)" )

message(STATUS "Build Target : ${BUILD_TARGET}")
message(STATUS "Build Platform : ${BUILD_PLATFORM}")

set(BUILDPATH ${CMAKE_BINARY_DIR})

if(${BUILD_TARGET} STREQUAL client)
    set(BUILDPATH "${BUILDPATH}/Client")
    add_compile_definitions(MONCRAFT_CLIENT)
    
    if(${BUILD_PLATFORM} STREQUAL wasm)
        set(BUILDPATH "${BUILDPATH}/Wasm")
    else()
        set(BUILDPATH "${BUILDPATH}/App")
    endif()

else()
    set(BUILDPATH "${BUILDPATH}/Server")
    add_compile_definitions(MONCRAFT_SERVER)
endif()

# Create build directories
file(MAKE_DIRECTORY ${BUILDPATH})
set(DEFAULT_BUILD_TYPE "Release")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILDPATH}/Release")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BUILDPATH}/Debug")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BUILDPATH}/Release")
message(STATUS "Written in ${BUILDPATH}")

if (UNIX)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/linux/")
else ()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/windows/")
endif ()


add_subdirectory(OpenGL)
