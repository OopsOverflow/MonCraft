set(SDL2_FOUND TRUE)
add_library(libpng16-16 SHARED IMPORTED GLOBAL)
add_library(SDL2 SHARED IMPORTED GLOBAL)
add_library(zlib1 SHARED IMPORTED GLOBAL)

set(SDL2_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/include")

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
    set(SDL2_LIBRARIES 
        "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.lib"
        "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2main.lib"
    )
    set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/libpng16-16.dll")
    set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.dll")
    set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/zlib1.dll")
else()
    set(SDL2_LIBRARIES 
        "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.lib" 
        "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2main.lib"
    )
    set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/libpng16-16.dll")
    set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.dll")
    set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/zlib1.dll")
endif()

mark_as_advanced(SDL2_LIBRARIES SDL2_INCLUDE_DIRS)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
$<TARGET_FILE:SDL2> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
$<TARGET_FILE:libpng16-16> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_if_different
$<TARGET_FILE:zlib1> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
