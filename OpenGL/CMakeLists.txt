# source files in common
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
	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/Chunk.cpp
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

# source files for the client
if(${BUILD_TARGET} STREQUAL client)
	message(STATUS "Configuring client cpp files")
	list(APPEND SOURCE
		${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/audio/Music.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/util/Raycast.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/util/Random.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/save/ClientConfig.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/Renderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/ChunkMesh.cpp

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

# source files for the server
else()
	message(STATUS "Configuring server cpp files")
	list(APPEND SOURCE
		${CMAKE_CURRENT_SOURCE_DIR}/src/Server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/Server.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/Client.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/UdpServer.cpp
	)
endif()

# source files for wasm client
if(${BUILD_PLATFORM} STREQUAL wasm)
	message(STATUS "Configuring wasm cpp files")
	list(APPEND SOURCE
		"${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/IpAddress.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Packet.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Socket.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/TcpSocket.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/UdpSocket.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Emscripten/SocketImpl.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Err.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Clock.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Sleep.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Time.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Unix/ClockImpl.cpp"
    "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Unix/SleepImpl.cpp"
	)
endif()

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE})

# libraries for windows
if(NOT UNIX)
	cmake_policy(SET CMP0111 NEW) # https://cmake.org/cmake/help/latest/policy/CMP0111.html

	#zlib
	add_library(zlib SHARED IMPORTED GLOBAL)
	set_property(TARGET zlib PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.lib")
	set_property(TARGET zlib PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/zlib/lib/zlib/zlib.dll")
	set_property(TARGET zlib PROPERTY INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/zlib/include/zlib")
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:zlib> $<TARGET_FILE_DIR:${PROJECT_NAME}>
	)

	#glew
	add_library(glew SHARED IMPORTED GLOBAL)
	set_property(TARGET zlib PROPERTY INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include")
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:glew> $<TARGET_FILE_DIR:${PROJECT_NAME}>
	)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
		set_property(TARGET glew PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64/glew32s.lib")
		set_property(TARGET glew PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/x64/glew32.dll")
	else()
		set_property(TARGET glew PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32/glew32s.lib")
		set_property(TARGET glew PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/GLEW/bin/Release/Win32/glew32s.dll")
	endif()

	#sdl2
	add_library(libpng16-16 SHARED IMPORTED GLOBAL)
	add_library(SDL2_image SHARED IMPORTED GLOBAL)
	add_library(SDL2 SHARED IMPORTED GLOBAL)
	add_library(SDL2main SHARED IMPORTED GLOBAL)
	add_library(zlib1 SHARED IMPORTED GLOBAL)
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
	if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
		set_property(TARGET SDL2_image PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2_image.lib")
		set_property(TARGET SDL2 PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.lib")
		set_property(TARGET SDL2main PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2main.lib")

		set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/libpng16-16.dll")
		set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2_image.dll")
		set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/SDL2.dll")
		set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x64/zlib1.dll")
	else()
		set_property(TARGET SDL2_image PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2_image.lib")
		set_property(TARGET SDL2 PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.lib")
		set_property(TARGET SDL2main PROPERTY IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2main.lib")

		set_property(TARGET libpng16-16 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/libpng16-16.dll")
		set_property(TARGET SDL2_image PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2_image.dll")
		set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/SDL2.dll")
		set_property(TARGET zlib1 PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/Dependencies/SDL2/lib/x86/zlib1.dll")
	endif()

	#sfml
	add_library(openal32 SHARED IMPORTED GLOBAL)
	add_library(sfml-audio SHARED IMPORTED GLOBAL)
	add_library(sfml-network SHARED IMPORTED GLOBAL)
	add_library(sfml-system SHARED IMPORTED GLOBAL)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio.lib")
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network.lib")
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system.lib")

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/openal32.dll")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-audio-2.dll")
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-network-2.dll")
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-system-2.dll")

		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/openal32.lib")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-audio-d.lib")
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-network-d.lib")
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x64/sfml-system-d.lib")

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/openal32.dll")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-audio-d-2.dll")
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-network-d-2.dll")
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x64/sfml-system-d-2.dll")
	else()
		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio.lib")
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network.lib")
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system.lib")

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/openal32.dll")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-audio-2.dll")
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-network-2.dll")
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-system-2.dll")

		set_property(TARGET openal32 PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/openal32.lib")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-audio-d.lib")
		set_property(TARGET sfml-network PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-network-d.lib")
		set_property(TARGET sfml-system PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/lib/x86/sfml-system-d.lib")

		set_property(TARGET openal32 PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/openal32.dll")
		set_property(TARGET sfml-audio PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-audio-d-2.dll")
		set_property(TARGET sfml-network PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-network-d-2.dll")
		set_property(TARGET sfml-system PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/SFML/bin/x86/sfml-system-d-2.dll")
	endif()

	#freetype
	add_library(freetype SHARED IMPORTED GLOBAL)
	set_property(TARGET zlib PROPERTY INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Dependencies/Freetype/include")
	if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64 bits
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib")
		set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_RELEASE

		)
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win64/freetype.lib")
		set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_DEBUG

		)
	else()
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib")
		set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_RELEASE

		)
		set_property(TARGET freetype PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_SOURCE_DIR}/Dependencies/FreeType/lib/win32/freetype.lib")
		# set_property(TARGET freetype PROPERTY IMPORTED_LOCATION_DEBUG)
	endif()
else()

endif()

# Define the include DIRs
message(STATUS "Configuring include directories")

# glm
include_directories("${CMAKE_SOURCE_DIR}/Dependencies/glm")

if (NOT ${BUILD_PLATFORM} STREQUAL wasm)
	find_package(ZLIB REQUIRED)
	include_directories(${ZLIB_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})

	find_package(GLEW REQUIRED)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

	find_package(SFML REQUIRED COMPONENTS system network)
	include_directories("${CMAKE_SOURCE_DIR}/Dependencies/SFML/include")
	target_link_libraries(${PROJECT_NAME} sfml-system sfml-network sfml-audio)

	find_package(Freetype REQUIRED)
	include_directories(${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})

	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
else ()
	include_directories("${CMAKE_SOURCE_DIR}/Dependencies/SFML/include")
	include_directories("${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src")

	if (${CMAKE_BUILD_TYPE} STREQUAL Release)
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
			"-fno-rtti \
			-Wno-pthreads-mem-growth \
			-pthread \
			-s PTHREAD_POOL_SIZE=5 \
			-s WASM=1 \
			-s USE_SDL=2 \
			-s USE_SDL_IMAGE=2 \
			-s SDL2_IMAGE_FORMATS='[\"png\"]' \
			-s USE_SDL_GFX=2 \
			-s USE_FREETYPE=2 \
			-s ALLOW_MEMORY_GROWTH=1 \
			-s MAX_WEBGL_VERSION=2 \
			-s MIN_WEBGL_VERSION=2 \
			-s FULL_ES2=1 \
			-s FULL_ES3=1 \
			-s WEBSOCKET_URL='wss://pi.thissma.fr:55001' \
			-s WEBSOCKET_SUBPROTOCOL='binary' \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/data@data \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/src@src \
			-Wno-unused-command-line-argument \
			-g0 -O3"
		)
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS
			"-fno-rtti \
			-Wno-pthreads-mem-growth \
			-pthread \
			-s PTHREAD_POOL_SIZE=5 \
			-s WASM=1 \
			-s USE_SDL=2 \
			-s USE_SDL_IMAGE=2 \
			-s SDL2_IMAGE_FORMATS='[\"png\"]' \
			-s USE_SDL_GFX=2 \
			-s USE_FREETYPE=2 \
			-s ALLOW_MEMORY_GROWTH=1 \
			-s MAX_WEBGL_VERSION=2 \
			-s MIN_WEBGL_VERSION=2 \
			-s FULL_ES2=1 \
			-s FULL_ES3=1 \
			-s WEBSOCKET_URL='wss://pi.thissma.fr:55001' \
			-s WEBSOCKET_SUBPROTOCOL='binary' \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/data@data \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/src@src \
			-Wno-unused-command-line-argument \
			-g0 -O3"
		)
	else ()
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
			"-fno-rtti \
			-Wno-pthreads-mem-growth \
			-pthread \
			-s PTHREAD_POOL_SIZE=5 \
			-s WASM=1 \
			-s USE_SDL=2 \
			-s USE_SDL_IMAGE=2 \
			-s SDL2_IMAGE_FORMATS='[\"png\"]' \
			-s USE_SDL_GFX=2 \
			-s USE_FREETYPE=2 \
			-s ALLOW_MEMORY_GROWTH=1 \
			-s MAX_WEBGL_VERSION=2 \
			-s MIN_WEBGL_VERSION=2 \
			-s FULL_ES2=1 \
			-s FULL_ES3=1 \
			-s WEBSOCKET_URL='wss://pi.thissma.fr:55001' \
			-s WEBSOCKET_SUBPROTOCOL='binary' \
			-s NO_DISABLE_EXCEPTION_CATCHING \
			-s NO_DISABLE_EXCEPTION_THROWING \
			-s DEMANGLE_SUPPORT=1 \
			-s ABORT_ON_WASM_EXCEPTIONS=1 \
			-s EXIT_RUNTIME=1 \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/data@data \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/src@src \
			-Wno-unused-command-line-argument \
			--source-map-base sources/build/Client/Wasm/Debug/ \
			-gsource-map -O0"
		)
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS
			"-fno-rtti \
			-Wno-pthreads-mem-growth \
			-pthread \
			-s PTHREAD_POOL_SIZE=5 \*
			-s WASM=1 \
			-s USE_SDL=2 \
			-s USE_SDL_IMAGE=2 \
			-s SDL2_IMAGE_FORMATS='[\"png\"]' \
			-s USE_SDL_GFX=2 \
			-s USE_FREETYPE=2 \
			-s ALLOW_MEMORY_GROWTH=1 \
			-s MAX_WEBGL_VERSION=2 \
			-s MIN_WEBGL_VERSION=2 \
			-s FULL_ES2=1 \
			-s FULL_ES3=1 \
			-s WEBSOCKET_URL='wss://pi.thissma.fr:55001' \
			-s WEBSOCKET_SUBPROTOCOL='binary' \
			-s NO_DISABLE_EXCEPTION_CATCHING \
			-s NO_DISABLE_EXCEPTION_THROWING \
			-s DEMANGLE_SUPPORT=1 \
			-s ABORT_ON_WASM_EXCEPTIONS=1 \
			-s EXIT_RUNTIME=1 \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/data@data \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/src@src \
			-Wno-unused-command-line-argument \
			--source-map-base sources/build/Client/Wasm/Debug/ \
			-gsource-map -O0"
		)
	endif ()

endif ()

# Copy assets
if (NOT ${BUILD_PLATFORM} STREQUAL wasm)
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/data/fonts $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/fonts
	)
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/data/music $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/music
	)
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/src/shader $<TARGET_FILE_DIR:${PROJECT_NAME}>/src/shader
	)
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/data/img $<TARGET_FILE_DIR:${PROJECT_NAME}>/data/img
	)
else ()
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/data/wasm/. $<TARGET_FILE_DIR:${PROJECT_NAME}>/
	)
	if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
		add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
			$<TARGET_FILE_DIR:${PROJECT_NAME}>/MonCraft.wasm.map $<TARGET_FILE_DIR:${PROJECT_NAME}>/sources/build/Client/Wasm/Debug/MonCraft.wasm.map
		)
		add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_CURRENT_SOURCE_DIR}/src $<TARGET_FILE_DIR:${PROJECT_NAME}>/sources/OpenGL/src
		)
	endif ()
endif ()
