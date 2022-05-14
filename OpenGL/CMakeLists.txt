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

# Define the include DIRs
if(${BUILD_PLATFORM} STREQUAL wasm)
	message(STATUS "Configuring wasm include directories")
	include_directories(
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/IpAddress.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Packet.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Socket.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/TcpSocket.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/UdpSocket.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Emscripten/SocketImpl.cpp

		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Err.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Clock.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Sleep.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Time.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Unix/ClockImpl.cpp
		${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Unix/SleepImpl.cpp
	)
else()
	message(STATUS "Configuring app include directories")
	include_directories(
		${CMAKE_SOURCE_DIR}/Dependencies/glm
	)

	if (UNIX)
		find_package(ZLIB REQUIRED)
		include_directories(${ZLIB_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
	else()
		include_directories(
			${PROJECT_NAME}
			${CMAKE_SOURCE_DIR}/Dependencies/zlib/include/zlib
			
		)
		target_link_libraries(
			${PROJECT_NAME}
			${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.lib
			${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlibstatic.lib
		)
	endif(UNIX)

	if(${BUILD_TARGET} STREQUAL client)

		cmake_policy(SET CMP0072 NEW) # OpenGL GLVND
		find_package(OpenGL REQUIRED)
		include_directories(${OPENGL_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

		if (UNIX)
			find_package(GLEW REQUIRED)
			include_directories(${GLEW_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include
				
			)
			target_link_libraries(${PROJECT_NAME} opengl32)
			if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
				target_link_libraries(
					${PROJECT_NAME}
					#${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32.lib
					${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib
				)
			elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)# 32 bits
				target_link_libraries(
					${PROJECT_NAME}
					#${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32/glew32.lib
					${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32/glew32s.lib
				)
			endif()
		endif(UNIX)

		if (UNIX)
			find_package(SDL2 REQUIRED)
			find_package(SDL2_image REQUIRED)
			include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/SDL2/include
			)
			if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
				target_link_libraries(
					${PROJECT_NAME}
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2_image.lib
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.lib
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2main.lib
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2test.lib
				)
			elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)# 32 bits
				target_link_libraries(
					${PROJECT_NAME}
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2_image.lib
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.lib
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2main.lib
					${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2test.lib
				)
			endif()
		endif(UNIX)

		if (UNIX)
			find_package(SFML REQUIRED COMPONENTS system network audio)
			target_link_libraries(${PROJECT_NAME} sfml-system sfml-network sfml-audio)
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/SFML/include
			)
			if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
				if(${CMAKE_BUILD_TYPE} STREQUAL Release)
					target_link_libraries(
						${PROJECT_NAME}
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib

						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio-s.lib
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio.lib

						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network-s.lib
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network.lib

						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system-s.lib
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system.lib
					)
				else()
					target_link_libraries(
						${PROJECT_NAME}
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib

						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio-d.lib
						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio-s-d.lib

						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network-d.lib
						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network-s-d.lib

						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system-d.lib
						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system-s-d.lib
					)
				endif()
				
			elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)# 32 bits
				if(${CMAKE_BUILD_TYPE} STREQUAL Release)
					target_link_libraries(
						${PROJECT_NAME}
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib

						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio-s.lib
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio.lib

						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network-s.lib
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network.lib

						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system-s.lib
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system.lib
					)
				else()
					target_link_libraries(
						${PROJECT_NAME}
						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib

						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio-d.lib
						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio-s-d.lib

						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network-d.lib
						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network-s-d.lib

						${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system-d.lib
						#${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system-s-d.lib
					)
				endif()
			endif()
		endif(UNIX)

		if (UNIX)
			find_package(Freetype REQUIRED)
			include_directories(${FREETYPE_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
		else()
			include_directories(
				${PROJECT_NAME}
				${CMAKE_SOURCE_DIR}/Dependencies/Freetype/include
				
			)
			if(CMAKE_SIZEOF_VOID_P EQUAL 8)# 64 bits
				target_link_libraries(
					${PROJECT_NAME}
					${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib
				)
			elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)# 32 bits
				target_link_libraries(
					${PROJECT_NAME}
					${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib
				)
			endif()
		endif(UNIX)

	else() #server

	endif()
endif()
