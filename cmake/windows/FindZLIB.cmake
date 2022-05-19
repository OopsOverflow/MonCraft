set(ZLIB_FOUND TRUE)
add_library(zlib SHARED IMPORTED GLOBAL)

set(ZLIB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Dependencies/zlib/include/zlib")

set(ZLIB_LIBRARIES "${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.lib")
set_property(TARGET zlib PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.dll")

mark_as_advanced(ZLIB_LIBRARIES ZLIB_INCLUDE_DIR)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:zlib> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
