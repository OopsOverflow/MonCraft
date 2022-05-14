# Add source files
set(SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/src/debug/Debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/debug/Bench.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/noise/value.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/noise/voronoi.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/noise/simplex.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/noise/XXHash.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/noise/prng.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/util/Identifier.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/util/Serde.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/save/SaveManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/save/ServerConfig.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Entity.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Hitbox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Node.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Entities.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/World.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/AbstractChunk.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/ChunkMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/BlockArray.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/AllBlocks.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/BlockModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/DefaultModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/TallgrassModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/WaterModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/CactusModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/OrientableModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/StairModel.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/NetworkError.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/Packet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/Serialize.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/terrain/TerrainGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/terrain/SliceMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/terrain/ChunkGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/terrain/Structure.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/terrain/BiomeMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/terrain/PendingChunks.cpp
)

if(${BUILD_TARGET} STREQUAL client)
	message(STATUS "Configuring client cpp files")
	list(APPEND SOURCE
		${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/audio/Music.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/util/Raycast.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/util/Random.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/save/ClientConfig.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/Renderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/Chunk.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Member.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/entity/character/Character.cpp
		
		${CMAKE_CURRENT_SOURCE_DIR}/src/controller/KeyboardController.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/controller/MouseController.cpp
		
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/widgets/MonCraftButton.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/widgets/ComboBox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/widgets/ParamList.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/widgets/RangeSlider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/widgets/KeySelector.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/widgets/Checkbox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/parametersMenu/AudioMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/parametersMenu/GraphicsMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/parametersMenu/KeyMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/parametersMenu/MiscMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/parametersMenu/ParametersMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/MainMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/MonCraftScene.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/GameMenu.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/Overlay.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/DebugOverlay.cpp
		
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Component.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Root.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Pane.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Text.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Button.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Input.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Event.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Image.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Key.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Box.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Grid.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/Slider.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/style/Style.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/style/Type.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui/style/Specification.cpp
		
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/client/Server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/client/ClientServer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/client/RealServer.cpp
		
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ShadowMap.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/Shader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/Camera.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/Viewport.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/Mesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/SafeMesh.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/Font.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/SkyBox.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/gl/ResourceManager.cpp
	)

else() # server
	message(STATUS "Configuring server cpp files")
	list(APPEND SOURCE
		${CMAKE_CURRENT_SOURCE_DIR}/src/Server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/Server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/UdpServer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/WebSocketServer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/Server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/Client.cpp
	)

endif()

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE})

if(NOT UNIX)
	cmake_policy(SET CMP0111 NEW)

	#zlib
	add_library(zlib SHARED IMPORTED GLOBAL)
	set_property(TARGET zlib PROPERTY IMPORTED_IMPLIB_RELEASE
		"${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.lib"
	)
	set_property(TARGET zlib PROPERTY IMPORTED_LOCATION_RELEASE
		"${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.dll"
	)
	set_property(TARGET zlib PROPERTY IMPORTED_IMPLIB_DEBUG
		"${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.lib"
	)
	set_property(TARGET zlib PROPERTY IMPORTED_LOCATION_DEBUG
		"${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.dll"
	)

	#glew
	add_library(glew SHARED IMPORTED GLOBAL)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
		set_property(TARGET glew PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib"
		)
		set_property(TARGET glew PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/x64/glew32.dll"
		)
		set_property(TARGET glew PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib"
		)
		set_property(TARGET glew PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/x64/glew32.dll"
		)
	else()
		set_property(TARGET glew PROPERTY IMPORTED_IMPLIB_RELEASE
		"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32/glew32s.lib"
		)
		set_property(TARGET glew PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/Win32/glew32s.dll"
		)
		set_property(TARGET glew PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32/glew32s.lib"
		)
		set_property(TARGET glew PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/Win32/glew32s.dll"
		)
	endif()

	#sdl2
	add_library(libpng16-16 SHARED IMPORTED GLOBAL)
	add_library(SDL2_image SHARED IMPORTED GLOBAL)
	add_library(SDL2 SHARED IMPORTED GLOBAL)
	add_library(SDL2main SHARED IMPORTED GLOBAL)
	add_library(zlib1 SHARED IMPORTED GLOBAL)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
		set_property(TARGET SDL2_image PROPERTY IMPORTED_IMPLIB_RELEASE 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2_image.lib"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_IMPLIB_RELEASE 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.lib"
		)
		set_property(TARGET SDL2main PROPERTY IMPORTED_IMPLIB_RELEASE 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2main.lib"
		)
			
		set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/libpng16-16.dll"
		)
		set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2_image.dll"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.dll"
		)
		set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/zlib1.dll"
		)
		
		set_property(TARGET SDL2_image PROPERTY IMPORTED_IMPLIB_DEBUG 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2_image.lib"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_IMPLIB_DEBUG 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.lib"
		)
		set_property(TARGET SDL2main PROPERTY IMPORTED_IMPLIB_DEBUG 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2main.lib"
		)
			
		set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/libpng16-16.dll"
		)
		set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2_image.dll"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.dll"
		)
		set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/zlib1.dll"
		)
	else()
		set_property(TARGET SDL2_image PROPERTY IMPORTED_IMPLIB_RELEASE 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2_image.lib"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_IMPLIB_RELEASE 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.lib"
		)
		set_property(TARGET SDL2main PROPERTY IMPORTED_IMPLIB_RELEASE 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2main.lib"
		)
			
		set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/libpng16-16.dll"
		)
		set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2_image.dll"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.dll"
		)
		set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/zlib1.dll"
		)
		
		set_property(TARGET SDL2_image PROPERTY IMPORTED_IMPLIB_DEBUG 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2_image.lib"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_IMPLIB_DEBUG 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.lib"
		)
		set_property(TARGET SDL2main PROPERTY IMPORTED_IMPLIB_DEBUG 
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2main.lib"
		)
			
		set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/libpng16-16.dll"
		)
		set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2_image.dll"
		)
		set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.dll"
		)
		set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/zlib1.dll"
		)
	endif()

	#sfml
	add_library(openal32 SHARED IMPORTED GLOBAL)
	add_library(sfml-audio SHARED IMPORTED GLOBAL)
	add_library(sfml-network SHARED IMPORTED GLOBAL)
	add_library(sfml-system SHARED IMPORTED GLOBAL)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio.lib"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network.lib"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system.lib"
		)

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/openal32.dll"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-audio-2.dll"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-network-2.dll"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-system-2.dll"
		)

		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio-d.lib"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network-d.lib"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system-d.lib"
		)

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/openal32.dll"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-audio-d-2.dll"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-network-d-2.dll"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-system-d-2.dll"
		)

	else()
		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_RELEASE
		"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio.lib"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network.lib"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system.lib"
		)

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/openal32.dll"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-audio-2.dll"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-network-2.dll"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-system-2.dll"
		)

		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio-d.lib"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network-d.lib"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system-d.lib"
		)

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/openal32.dll"
		)
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-audio-d-2.dll"
		)
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-network-d-2.dll"
		)
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-system-d-2.dll"
		)
	endif()

	#freetype
	add_library(freetype SHARED IMPORTED GLOBAL)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib"
		)
		set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_RELEASE
		
		)
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib"
		)
		set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_DEBUG

		)
	else()
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_RELEASE
			"${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib"
		)
		set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_RELEASE

		)
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_DEBUG
			"${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib"
		)
		set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_DEBUG

		)
	endif()

endif()

# Define the include DIRs
if(${BUILD_PLATFORM} STREQUAL wasm)
	message(STATUS "Configuring wasm include directories")
	include_directories(
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System
	)
else()
	message(STATUS "Configuring app include directories")

	include_directories(
		${CMAKE_SOURCE_DIR}/Dependencies/glm
	)

	if (UNIX)#zlib
		find_package(ZLIB REQUIRED)
		include_directories(${ZLIB_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
	else()
		include_directories(
			${PROJECT_NAME}
			${CMAKE_SOURCE_DIR}/Dependencies/zlib/include/zlib
		)
		target_link_libraries(${PROJECT_NAME} zlib)
		
		add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
    	COMMAND ${CMAKE_COMMAND} -E copy_if_different
    	$<TARGET_FILE:zlib> $<TARGET_FILE_DIR:${PROJECT_NAME}>
		)
	endif(UNIX)

	if(${BUILD_TARGET} STREQUAL client)

		cmake_policy(SET CMP0072 NEW) # OpenGL GLVND
		find_package(OpenGL REQUIRED)
		include_directories(${OPENGL_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

		if (UNIX)#glew
			find_package(GLEW REQUIRED)
			include_directories(${GLEW_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include
			)
			target_link_libraries(${PROJECT_NAME} glew)

			add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:glew> $<TARGET_FILE_DIR:${PROJECT_NAME}>
			)
		endif(UNIX)

		if (UNIX)#sdl2
			find_package(SDL2 REQUIRED)
			find_package(SDL2_image REQUIRED)
			include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/SDL2/include
			)
			target_link_libraries(${PROJECT_NAME} SDL2_image SDL2 SDL2main)

			add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL2_image> $<TARGET_FILE_DIR:${PROJECT_NAME}>
			)

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
			
		endif(UNIX)

		if (UNIX)#sfml
			find_package(SFML REQUIRED COMPONENTS system network audio)
			target_link_libraries(${PROJECT_NAME} sfml-system sfml-network sfml-audio)
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/SFML/include
			)
			target_link_libraries(${PROJECT_NAME} openal32 sfml-audio sfml-network sfml-system)

			add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:openal32> $<TARGET_FILE_DIR:${PROJECT_NAME}>
			)

			add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:sfml-audio> $<TARGET_FILE_DIR:${PROJECT_NAME}>
			)

			add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:sfml-network> $<TARGET_FILE_DIR:${PROJECT_NAME}>
			)

			add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:sfml-system> $<TARGET_FILE_DIR:${PROJECT_NAME}>
			)

		endif(UNIX)

		if (UNIX)#freetype
			find_package(Freetype REQUIRED)
			include_directories(${FREETYPE_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/Freetype/include
			)
			target_link_libraries(${PROJECT_NAME} freetype)

		endif(UNIX)

		#add missing folders
		add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_CURRENT_SOURCE_DIR}/data/img $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
		)

		add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_CURRENT_SOURCE_DIR}/data/music $<TARGET_FILE_DIR:${PROJECT_NAME}>/data
		)

		add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_CURRENT_SOURCE_DIR}/src/shader $<TARGET_FILE_DIR:${PROJECT_NAME}>/src/shader
		)
	else() #server

	endif()
endif()

