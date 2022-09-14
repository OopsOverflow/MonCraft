set(DATACHANNEL_FOUND TRUE)
add_library(datachannel SHARED IMPORTED GLOBAL)

set(DATACHANNEL_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Dependencies/libdatachannel/include")

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
  set_property(TARGET datachannel PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/libdatachannel/lib/libdatachannel.so")
else()
  message(FATAL_ERROR "TODO")
endif()

mark_as_advanced(DATACHANNEL_LIBRARIES DATACHANNEL_INCLUDE_DIR)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:datachannel> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
