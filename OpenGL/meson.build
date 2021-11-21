project('MonCraft', 'cpp', default_options : ['cpp_std=c++17'])

if host_machine.system() != 'emscripten'
  add_project_link_arguments(['-lstdc++fs'], language: 'cpp')
endif

if get_option('buildtype').startswith('debug')
  add_project_arguments('-DDEBUG', language : 'cpp')
endif

srcs = files(
  'OpenGL/src/debug/Debug.cpp',
  'OpenGL/src/debug/Bench.cpp',

  'OpenGL/src/noise/value.cpp',
  'OpenGL/src/noise/voronoi.cpp',
  'OpenGL/src/noise/simplex.cpp',
  'OpenGL/src/noise/XXHash.cpp',
  'OpenGL/src/noise/prng.cpp',

  'OpenGL/src/util/Identifier.cpp',
  'OpenGL/src/util/Serde.cpp',

  'OpenGL/src/save/SaveManager.cpp',
  'OpenGL/src/save/ServerConfig.cpp',

  'OpenGL/src/entity/Entity.cpp',
  'OpenGL/src/entity/Hitbox.cpp',
  'OpenGL/src/entity/Node.cpp',
  'OpenGL/src/entity/Entities.cpp',

  'OpenGL/src/terrain/World.cpp',
  'OpenGL/src/terrain/AbstractChunk.cpp',
  'OpenGL/src/terrain/ChunkMap.cpp',
  'OpenGL/src/terrain/BlockArray.cpp',

  'OpenGL/src/blocks/AllBlocks.cpp',
  'OpenGL/src/blocks/model/BlockModel.cpp',
  'OpenGL/src/blocks/model/DefaultModel.cpp',
  'OpenGL/src/blocks/model/TallgrassModel.cpp',
  'OpenGL/src/blocks/model/WaterModel.cpp',
  'OpenGL/src/blocks/model/CactusModel.cpp',
  'OpenGL/src/blocks/model/OrientableModel.cpp',
  'OpenGL/src/blocks/model/StairModel.cpp',

  'OpenGL/src/multiplayer/NetworkError.cpp',
  'OpenGL/src/multiplayer/Packet.cpp',
  'OpenGL/src/multiplayer/Serialize.cpp',

  'OpenGL/src/multiplayer/terrain/TerrainGenerator.cpp',
  'OpenGL/src/multiplayer/terrain/SliceMap.cpp',
  'OpenGL/src/multiplayer/terrain/ChunkGenerator.cpp',
  'OpenGL/src/multiplayer/terrain/Structure.cpp',
  'OpenGL/src/multiplayer/terrain/BiomeMap.cpp',
  'OpenGL/src/multiplayer/terrain/PendingChunks.cpp',
)

client_srcs = files(
  'OpenGL/src/Application.cpp',

  'OpenGL/src/util/Raycast.cpp',
  'OpenGL/src/util/Random.cpp',

  'OpenGL/src/save/ClientConfig.cpp',

  'OpenGL/src/terrain/Renderer.cpp',
  'OpenGL/src/terrain/Chunk.cpp',

  'OpenGL/src/entity/Member.cpp',
  'OpenGL/src/entity/character/Character.cpp',

  'OpenGL/src/controller/KeyboardController.cpp',
  'OpenGL/src/controller/MouseController.cpp',

  'OpenGL/src/interface/widgets/MonCraftButton.cpp',
  'OpenGL/src/interface/widgets/ComboBox.cpp',
  'OpenGL/src/interface/widgets/ParamList.cpp',
  'OpenGL/src/interface/widgets/RangeSlider.cpp',
  'OpenGL/src/interface/widgets/KeySelector.cpp',
  'OpenGL/src/interface/widgets/Checkbox.cpp',
  'OpenGL/src/interface/parametersMenu/AudioMenu.cpp',
  'OpenGL/src/interface/parametersMenu/GraphicsMenu.cpp',
  'OpenGL/src/interface/parametersMenu/KeyMenu.cpp',
  'OpenGL/src/interface/parametersMenu/MiscMenu.cpp',
  'OpenGL/src/interface/parametersMenu/ParametersMenu.cpp',
  'OpenGL/src/interface/MainMenu.cpp',
  'OpenGL/src/interface/MonCraftScene.cpp',
  'OpenGL/src/interface/GameMenu.cpp',
  'OpenGL/src/interface/Overlay.cpp',
  'OpenGL/src/interface/DebugOverlay.cpp',

  'OpenGL/src/ui/Component.cpp',
  'OpenGL/src/ui/Root.cpp',
  'OpenGL/src/ui/Pane.cpp',
  'OpenGL/src/ui/Text.cpp',
  'OpenGL/src/ui/Button.cpp',
  'OpenGL/src/ui/Input.cpp',
  'OpenGL/src/ui/Event.cpp',
  'OpenGL/src/ui/Image.cpp',
  'OpenGL/src/ui/Key.cpp',
  'OpenGL/src/ui/Box.cpp',
  'OpenGL/src/ui/Grid.cpp',
  'OpenGL/src/ui/Slider.cpp',
  'OpenGL/src/ui/style/Style.cpp',
  'OpenGL/src/ui/style/Type.cpp',
  'OpenGL/src/ui/style/Specification.cpp',

  'OpenGL/src/multiplayer/client/Server.cpp',
  'OpenGL/src/multiplayer/client/ClientServer.cpp',
  'OpenGL/src/multiplayer/client/RealServer.cpp',

  'OpenGL/src/gl/ShadowMap.cpp',
  'OpenGL/src/gl/Shader.cpp',
  'OpenGL/src/gl/Camera.cpp',
  'OpenGL/src/gl/Viewport.cpp',
  'OpenGL/src/gl/Mesh.cpp',
  'OpenGL/src/gl/SafeMesh.cpp',
  'OpenGL/src/gl/Font.cpp',
  'OpenGL/src/gl/SkyBox.cpp',
  'OpenGL/src/gl/ResourceManager.cpp',
)

server_srcs = files(
  'OpenGL/src/Server.cpp',
  'OpenGL/src/multiplayer/server/Server.cpp',
  'OpenGL/src/multiplayer/server/UdpServer.cpp',
  'OpenGL/src/multiplayer/server/WebSocketServer.cpp',
  'OpenGL/src/multiplayer/server/Server.cpp',
  'OpenGL/src/multiplayer/server/Client.cpp',
)

debug_srcs = files(
  'OpenGL/src/noise/bitmapTex.cpp',
)

if host_machine.system() == 'emscripten'
  incs = include_directories('OpenGL/src', 'Dependencies', 'Dependencies/SFML/include', 'Dependencies/EMSFML/src/')

  emsfml_srcs = files(
    'Dependencies/EMSFML/src/SFML/Network/IpAddress.cpp',
    'Dependencies/EMSFML/src/SFML/Network/Packet.cpp',
    'Dependencies/EMSFML/src/SFML/Network/Socket.cpp',
    'Dependencies/EMSFML/src/SFML/Network/TcpSocket.cpp',
    'Dependencies/EMSFML/src/SFML/Network/UdpSocket.cpp',
    'Dependencies/EMSFML/src/SFML/Network/Emscripten/SocketImpl.cpp',

    'Dependencies/EMSFML/src/SFML/System/Err.cpp',
    'Dependencies/EMSFML/src/SFML/System/Clock.cpp',
    'Dependencies/EMSFML/src/SFML/System/Sleep.cpp',
    'Dependencies/EMSFML/src/SFML/System/Time.cpp',
    'Dependencies/EMSFML/src/SFML/System/Unix/ClockImpl.cpp',
    'Dependencies/EMSFML/src/SFML/System/Unix/SleepImpl.cpp',
  )

  exe = executable(
    'MonCraft', [emsfml_srcs, client_srcs, srcs],
    cpp_args: ['-DMONCRAFT_CLIENT'],
    include_directories: incs
  )

else
  deps = [
    dependency('glm'),
    dependency('threads'),
    dependency('zlib'),
    dependency('sfml-network'),
  ]

  client_deps = [
    dependency('glew'),
    dependency('gl'),
    dependency('sdl2'),
    dependency('SDL2_image'),
    dependency('sfml-audio'),
    dependency('freetype2'),
  ]

  server_deps = [
    dependency('openssl'),
    dependency('boost', modules : ['system']),
  ]

  client_srcs += files('OpenGL/src/audio/Music.cpp')

  incs = include_directories('OpenGL/src', 'Dependencies')

  # install_subdir('OpenGL/src/shader', install_dir : 'shader')

  exe = executable(
    'MonCraft', [client_srcs, srcs],
    cpp_args: ['-DMONCRAFT_CLIENT'],
    dependencies: [deps, client_deps],
    include_directories: incs
  )

  # exe_server = executable(
  #   'MonCraftServer', [server_srcs, srcs],
  #   cpp_args: ['-DMONCRAFT_SERVER'],
  #   dependencies: [deps, server_deps],
  #   include_directories: incs
  # )
endif
