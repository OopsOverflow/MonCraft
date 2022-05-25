set(FREETYPE_FOUND TRUE)
set(FREETYPE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Dependencies/Freetype/include")
if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
    set(FREETYPE_LIBRARIES "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib")
else()
    set(FREETYPE_LIBRARIES "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib")
endif()
mark_as_advanced(FREETYPE_LIBRARIES FREETYPE_INCLUDE_DIRS)



# #freetype
# add_library(freetype SHARED IMPORTED GLOBAL)
# set_property(TARGET freetype PROPERTY INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/Freetype/include")
# if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
#     set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib")
#     set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_RELEASE

#     )
#     set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib")
#     set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_DEBUG

#     )
# else()
#     set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib")
#     set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_RELEASE

#     )
#     set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib")
#     # set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_DEBUG)
# endif()