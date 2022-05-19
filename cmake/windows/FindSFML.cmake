set(SFML_FOUND TRUE)
add_library(openal32 SHARED IMPORTED GLOBAL)
add_library(sfml-audio SHARED IMPORTED GLOBAL)
add_library(sfml-network SHARED IMPORTED GLOBAL)
add_library(sfml-system SHARED IMPORTED GLOBAL)

set(SFML_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Dependencies/SFML/include")

if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits

	set(SFML_LIBRARIES
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio.lib" 
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system.lib"

		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio-d.lib" 
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network-d.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system-d.lib"
	)

	set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/openal32.dll")
	set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-audio-2.dll")
	set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-network-2.dll")
	set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-system-2.dll")

	set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/openal32.dll")
	set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-audio-d-2.dll")
	set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-network-d-2.dll")
	set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-system-d-2.dll")

else()

	set(SFML_LIBRARIES
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio.lib" 
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system.lib"

		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio-d.lib" 
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network-d.lib"
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system-d.lib"
	)

	
	set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/openal32.dll")
	set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-audio-2.dll")
	set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-network-2.dll")
	set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-system-2.dll")

	set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/openal32.dll")
	set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-audio-d-2.dll")
	set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-network-d-2.dll")
	set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-system-d-2.dll")

endif()

mark_as_advanced(SFML_LIBRARIES SFML_INCLUDE_DIR)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:openal32> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:sfml-network> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:sfml-system> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:sfml-audio> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)


