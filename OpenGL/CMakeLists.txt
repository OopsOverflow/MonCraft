cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

# set the project name
set(PROJECT_NAME "MonCraft")
project(${PROJECT_NAME} VERSION 1.1.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CMAKE_BUILD_TYPE "Choose between debug and release" release)
option(BUILD_TARGET "Choose build target client/server" client)
option(EMSCRIPTEN "Enable Emscripten ON/OFF" OFF)

# Add source files
file(WRITE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/debug/Debug.cpp
	${CMAKE_SOURCE_DIR}/src/debug/Bench.cpp

	${CMAKE_SOURCE_DIR}/src/noise/value.cpp
	${CMAKE_SOURCE_DIR}/src/noise/voronoi.cpp
	${CMAKE_SOURCE_DIR}/src/noise/simplex.cpp
	${CMAKE_SOURCE_DIR}/src/noise/XXHash.cpp
	${CMAKE_SOURCE_DIR}/src/noise/prng.cpp

	${CMAKE_SOURCE_DIR}/src/util/Identifier.cpp
	${CMAKE_SOURCE_DIR}/src/util/Serde.cpp

	${CMAKE_SOURCE_DIR}/src/save/SaveManager.cpp
	${CMAKE_SOURCE_DIR}/src/save/ServerConfig.cpp

	${CMAKE_SOURCE_DIR}/src/entity/Entity.cpp
	${CMAKE_SOURCE_DIR}/src/entity/Hitbox.cpp
	${CMAKE_SOURCE_DIR}/src/entity/Node.cpp
	${CMAKE_SOURCE_DIR}/src/entity/Entities.cpp

	${CMAKE_SOURCE_DIR}/src/terrain/World.cpp
	${CMAKE_SOURCE_DIR}/src/terrain/AbstractChunk.cpp
	${CMAKE_SOURCE_DIR}/src/terrain/ChunkMap.cpp
	${CMAKE_SOURCE_DIR}/src/terrain/BlockArray.cpp

	${CMAKE_SOURCE_DIR}/src/blocks/AllBlocks.cpp
	${CMAKE_SOURCE_DIR}/src/blocks/model/BlockModel.cpp
	${CMAKE_SOURCE_DIR}/src/blocks/model/DefaultModel.cpp
	${CMAKE_SOURCE_DIR}/src/blocks/model/TallgrassModel.cpp
	${CMAKE_SOURCE_DIR}/src/blocks/model/WaterModel.cpp
	${CMAKE_SOURCE_DIR}/src/blocks/model/CactusModel.cpp
	${CMAKE_SOURCE_DIR}/src/blocks/model/OrientableModel.cpp
	${CMAKE_SOURCE_DIR}/src/blocks/model/StairModel.cpp

	${CMAKE_SOURCE_DIR}/src/multiplayer/NetworkError.cpp
	${CMAKE_SOURCE_DIR}/src/multiplayer/Packet.cpp
	${CMAKE_SOURCE_DIR}/src/multiplayer/Serialize.cpp

	${CMAKE_SOURCE_DIR}/src/multiplayer/terrain/TerrainGenerator.cpp
	${CMAKE_SOURCE_DIR}/src/multiplayer/terrain/SliceMap.cpp
	${CMAKE_SOURCE_DIR}/src/multiplayer/terrain/ChunkGenerator.cpp
	${CMAKE_SOURCE_DIR}/src/multiplayer/terrain/Structure.cpp
	${CMAKE_SOURCE_DIR}/src/multiplayer/terrain/BiomeMap.cpp
	${CMAKE_SOURCE_DIR}/src/multiplayer/terrain/PendingChunks.cpp
)

if(BUILD_TARGET EQUAL client)
	file(APPEND SOURCE_FILES 
		${CMAKE_SOURCE_DIR}/src/Application.cpp

		${CMAKE_SOURCE_DIR}/src/util/Raycast.cpp
		${CMAKE_SOURCE_DIR}/src/util/Random.cpp

		${CMAKE_SOURCE_DIR}/src/save/ClientConfig.cpp

		${CMAKE_SOURCE_DIR}/src/terrain/Renderer.cpp
		${CMAKE_SOURCE_DIR}/src/terrain/Chunk.cpp

		${CMAKE_SOURCE_DIR}/src/entity/Member.cpp
		${CMAKE_SOURCE_DIR}/src/entity/character/Character.cpp
		
		${CMAKE_SOURCE_DIR}/src/controller/KeyboardController.cpp
		${CMAKE_SOURCE_DIR}/src/controller/MouseController.cpp
		
		${CMAKE_SOURCE_DIR}/src/interface/widgets/MonCraftButton.cpp
		${CMAKE_SOURCE_DIR}/src/interface/widgets/ComboBox.cpp
		${CMAKE_SOURCE_DIR}/src/interface/widgets/ParamList.cpp
		${CMAKE_SOURCE_DIR}/src/interface/widgets/RangeSlider.cpp
		${CMAKE_SOURCE_DIR}/src/interface/widgets/KeySelector.cpp
		${CMAKE_SOURCE_DIR}/src/interface/widgets/Checkbox.cpp
		${CMAKE_SOURCE_DIR}/src/interface/parametersMenu/AudioMenu.cpp
		${CMAKE_SOURCE_DIR}/src/interface/parametersMenu/GraphicsMenu.cpp
		${CMAKE_SOURCE_DIR}/src/interface/parametersMenu/KeyMenu.cpp
		${CMAKE_SOURCE_DIR}/src/interface/parametersMenu/MiscMenu.cpp
		${CMAKE_SOURCE_DIR}/src/interface/parametersMenu/ParametersMenu.cpp
		${CMAKE_SOURCE_DIR}/src/interface/MainMenu.cpp
		${CMAKE_SOURCE_DIR}/src/interface/MonCraftScene.cpp
		${CMAKE_SOURCE_DIR}/src/interface/GameMenu.cpp
		${CMAKE_SOURCE_DIR}/src/interface/Overlay.cpp
		${CMAKE_SOURCE_DIR}/src/interface/DebugOverlay.cpp
		
		${CMAKE_SOURCE_DIR}/src/ui/Component.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Root.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Pane.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Text.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Button.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Input.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Event.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Image.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Key.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Box.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Grid.cpp
		${CMAKE_SOURCE_DIR}/src/ui/Slider.cpp
		${CMAKE_SOURCE_DIR}/src/ui/style/Style.cpp
		${CMAKE_SOURCE_DIR}/src/ui/style/Type.cpp
		${CMAKE_SOURCE_DIR}/src/ui/style/Specification.cpp
		
		${CMAKE_SOURCE_DIR}/src/multiplayer/client/Server.cpp
		${CMAKE_SOURCE_DIR}/src/multiplayer/client/ClientServer.cpp
		${CMAKE_SOURCE_DIR}/src/multiplayer/client/RealServer.cpp
		
		${CMAKE_SOURCE_DIR}/src/gl/ShadowMap.cpp
		${CMAKE_SOURCE_DIR}/src/gl/Shader.cpp
		${CMAKE_SOURCE_DIR}/src/gl/Camera.cpp
		${CMAKE_SOURCE_DIR}/src/gl/Viewport.cpp
		${CMAKE_SOURCE_DIR}/src/gl/Mesh.cpp
		${CMAKE_SOURCE_DIR}/src/gl/SafeMesh.cpp
		${CMAKE_SOURCE_DIR}/src/gl/Font.cpp
		${CMAKE_SOURCE_DIR}/src/gl/SkyBox.cpp
		${CMAKE_SOURCE_DIR}/src/gl/ResourceManager.cpp
	)

else() # server

	file(APPEND SOURCE_FILES 
		${CMAKE_SOURCE_DIR}/src/Server.cpp
		${CMAKE_SOURCE_DIR}/src/multiplayer/server/Server.cpp
		${CMAKE_SOURCE_DIR}/src/multiplayer/server/UdpServer.cpp
		${CMAKE_SOURCE_DIR}/src/multiplayer/server/WebSocketServer.cpp
		${CMAKE_SOURCE_DIR}/src/multiplayer/server/Server.cpp
		${CMAKE_SOURCE_DIR}/src/multiplayer/server/Client.cpp
	)

endif(BUILD_TARGET)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Define the include DIRs
if(EMSCRIPTEN)
	include_directories(
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/IpAddress.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Packet.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Socket.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/TcpSocket.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/UdpSocket.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/Network/Emscripten/SocketImpl.cpp

		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Err.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Clock.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Sleep.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Time.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Unix/ClockImpl.cpp
		${PROJECT_SOURCE_DIR}/Dependencies/EMSFML/src/SFML/System/Unix/SleepImpl.cpp
	)
else()
	include_directories(
		${PROJECT_SOURCE_DIR}/Dependencies/glm
	)
	if(BUILD_TARGET EQUAL client)
		include_directories(
			${PROJECT_SOURCE_DIR}/Dependencies/GLEW
			${PROJECT_SOURCE_DIR}/Dependencies/SDL2
			${PROJECT_SOURCE_DIR}/Dependencies/SFML
			${PROJECT_SOURCE_DIR}/Dependencies/FreeType
		)

	else() #server
		include_directories(
			${PROJECT_SOURCE_DIR}/Dependencies/GLEW
			${PROJECT_SOURCE_DIR}/Dependencies/SDL2
		)

	endif(BUILD_TARGET)
endif(EMSCRIPTEN)
