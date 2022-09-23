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
	${CMAKE_CURRENT_SOURCE_DIR}/src/util/Raycast.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/save/SaveManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/save/ServerConfig.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Entity.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Hitbox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Node.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Entities.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/entity/character/Character.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/World.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/Chunk.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/ChunkMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/BlockArray.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/AllBlocks.cpp

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

		${CMAKE_CURRENT_SOURCE_DIR}/src/util/Random.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/util/Bezier.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/util/Spline.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/util/AnimationMixer.cpp
		
		${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/BlockModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/DefaultModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/TallgrassModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/WaterModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/CactusModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/OrientableModel.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/blocks/model/StairModel.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/save/ClientConfig.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/Renderer.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/terrain/ChunkMesh.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/entity/Member.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/entity/character/CharacterMesh.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/controller/AIController.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/controller/RemotePlayerController.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/controller/PlayerController.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/controller/MouseController.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/controller/Controller.cpp

		${CMAKE_CURRENT_SOURCE_DIR}/src/interface/widgets/BlockSelector.cpp
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
		${CMAKE_CURRENT_SOURCE_DIR}/src/multiplayer/server/WebSocketServer.cpp
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

message(STATUS "Configuring dependencies")

# Define the include DIRs
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/OpenGL/src")

# glm
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies/glm")

# spdlog
if(${BUILD_PLATFORM} STREQUAL wasm)
	add_subdirectory("${CMAKE_SOURCE_DIR}/Dependencies/spdlog" "spdlog" EXCLUDE_FROM_ALL)
	target_compile_options(spdlog PUBLIC -pthread)
	target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
else ()
	find_package(spdlog REQUIRED)
	target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
endif ()

# libdatachannel
if(${BUILD_PLATFORM} STREQUAL wasm)
	add_subdirectory("${CMAKE_SOURCE_DIR}/Dependencies/datachannel-wasm" "datachannel-wasm" EXCLUDE_FROM_ALL)
	target_link_libraries(${PROJECT_NAME} datachannel-wasm)
elseif (UNIX)
	add_subdirectory("${CMAKE_SOURCE_DIR}/Dependencies/libdatachannel" "libdatachannel" EXCLUDE_FROM_ALL)
	target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies/libdatachannel/include")
	target_link_libraries(${PROJECT_NAME} datachannel)
else () # WIN32
	find_package(LIBDATACHANNEL REQUIRED)
	target_link_libraries(${PROJECT_NAME} LibDataChannel::LibDataChannel)
endif()

if (NOT ${BUILD_PLATFORM} STREQUAL wasm)
	# zlib
	find_package(ZLIB REQUIRED)
	target_link_libraries(${PROJECT_NAME} ZLIB::ZLIB)


	if(${BUILD_TARGET} STREQUAL client)
		find_package(SFML REQUIRED COMPONENTS system network audio)
		target_link_libraries(${PROJECT_NAME} sfml-system sfml-network sfml-audio)

		find_package(OpenGL REQUIRED)
		target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${OPENGL_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
		
		find_package(GLEW REQUIRED)
    target_compile_definitions(${PROJECT_NAME} PUBLIC GLEW_STATIC)
		target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${GLEW_INCLUDE_DIRS})
		target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

		if (UNIX)
			find_package(Freetype REQUIRED)
			target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${FREETYPE_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} freetype)
		else ()
			find_package(Freetype CONFIG REQUIRED)
			target_link_libraries(${PROJECT_NAME} freetype)
		endif ()

		if (UNIX)
			find_package(SDL2 REQUIRED)
			find_package(SDL2_image REQUIRED)
			target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
			target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
		else ()
			find_package(SDL2 REQUIRED)
			find_package(sdl2-image REQUIRED)
			target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main SDL2::SDL2_image)
		endif ()

	else ()
		find_package(SFML REQUIRED COMPONENTS system network)
		target_link_libraries(${PROJECT_NAME} sfml-system sfml-network)
	endif()

else () # wasm
	target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/include")
	target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "${CMAKE_SOURCE_DIR}/Dependencies/EMSFML/src")

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
			-s USE_FREETYPE=1 \
			-s ALLOW_MEMORY_GROWTH=1 \
			-s MAX_WEBGL_VERSION=2 \
			-s MIN_WEBGL_VERSION=2 \
			-s FULL_ES2=1 \
			-s FULL_ES3=1 \
			-s WEBSOCKET_URL='wss://pi.thissma.fr:55001' \
			-s WEBSOCKET_SUBPROTOCOL='binary' \
			-s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=wsSendMessage \
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
			-s USE_FREETYPE=1 \
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
	else () # Debug
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS
			"-fno-rtti \
			-Wno-pthreads-mem-growth \
			-pthread \
			-fsanitize=undefined \
			-fsanitize=address \
			-s PTHREAD_POOL_SIZE=5 \
			-s WASM=1 \
			-s USE_SDL=2 \
			-s USE_SDL_IMAGE=2 \
			-s SDL2_IMAGE_FORMATS='[\"png\"]' \
			-s USE_SDL_GFX=2 \
			-s USE_FREETYPE=1 \
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
			-s DEFAULT_LIBRARY_FUNCS_TO_INCLUDE=wsSendMessage \
			-s ASSERTIONS=2 \
			-s STACK_OVERFLOW_CHECK=2 \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/data@data \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/src@src \
			-Wno-unused-command-line-argument \
			--source-map-base sources/build/Client/Wasm/Debug/ \
			-gsource-map -O0"
			# -s SAFE_HEAP=1 \
		)
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS
			"-fno-rtti \
			-Wno-pthreads-mem-growth \
			-pthread \
			-fsanitize=undefined \
			-fsanitize=address \
			-s PTHREAD_POOL_SIZE=5 \
			-s WASM=1 \
			-s USE_SDL=2 \
			-s USE_SDL_IMAGE=2 \
			-s SDL2_IMAGE_FORMATS='[\"png\"]' \
			-s USE_SDL_GFX=2 \
			-s USE_FREETYPE=1 \
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
			-s ASSERTIONS=2 \
			-s STACK_OVERFLOW_CHECK=2 \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/data@data \
			--preload-file ${CMAKE_SOURCE_DIR}/OpenGL/src@src \
			-Wno-unused-command-line-argument \
			--source-map-base sources/build/Client/Wasm/Debug/ \
			-gsource-map -O0"
			# -s SAFE_HEAP=1 \
		)
	endif ()

endif ()


if (NOT ${BUILD_PLATFORM} STREQUAL wasm)
	# Force shaders to be copied
	add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E rm -rf
			$<TARGET_FILE_DIR:${PROJECT_NAME}>/src
	)

	# Copy assets
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
		add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
				${CMAKE_SOURCE_DIR}/Dependencies/datachannel-wasm $<TARGET_FILE_DIR:${PROJECT_NAME}>/sources/Dependencies/datachannel-wasm
		)
	endif ()
endif ()

# configure logger
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG>")
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

# ASan / UBSan
if (${ENABLE_SANITIZERS})
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
	set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined")
endif ()
