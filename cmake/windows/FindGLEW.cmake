set(GLEW_FOUND TRUE)

set(GLEW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include")

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
    set(GLEW_LIBRARIES "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib")
else()
    set(GLEW_LIBRARIES "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32/glew32s.lib")
endif()

mark_as_advanced(GLEW_LIBRARIES GLEW_INCLUDE_DIR)