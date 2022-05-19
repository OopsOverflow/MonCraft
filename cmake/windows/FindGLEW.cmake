set(GLEW_FOUND TRUE)
add_library(glew SHARED IMPORTED GLOBAL)

set(GLEW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include")

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
    set(GLEW_LIBRARIES "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib")
    set_property(TARGET glew PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/x64/glew32.dll")
else()
    set(GLEW_LIBRARIES "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32/glew32s.lib")
    set_property(TARGET glew PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/Win32/glew32s.dll")
endif()

mark_as_advanced(GLEW_LIBRARIES GLEW_INCLUDE_DIR)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
$<TARGET_FILE:glew> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)