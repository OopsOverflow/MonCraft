project('MonCraft', 'cpp')

srcs = files(
  'OpenGL/src/Application.cpp',

  'OpenGL/src/audio/Music.cpp',
  'OpenGL/src/debug/Debug.cpp',

  'OpenGL/src/controller/KeyboardController.cpp',
  'OpenGL/src/controller/MouseController.cpp',

  'OpenGL/src/noise/bitmapTex.cpp',
  'OpenGL/src/noise/value.cpp',
  'OpenGL/src/noise/voronoi.cpp',
  'OpenGL/src/noise/simplex.cpp',

  'OpenGL/src/gl/ShadowMap.cpp',
  'OpenGL/src/gl/Shader.cpp',
  'OpenGL/src/gl/Camera.cpp',
  'OpenGL/src/gl/Viewport.cpp',
  'OpenGL/src/gl/Mesh.cpp',
  'OpenGL/src/gl/Loader.cpp',

  'OpenGL/src/util/Raycast.cpp',
  'OpenGL/src/util/Random.cpp',

  'OpenGL/src/entity/Entity.cpp',
  'OpenGL/src/entity/Hitbox.cpp',
  'OpenGL/src/entity/Member.cpp',
  'OpenGL/src/entity/character/Character.cpp',

  'OpenGL/src/terrain/Terrain.cpp',
  'OpenGL/src/terrain/Chunk.cpp',
  'OpenGL/src/terrain/Generator.cpp',
  'OpenGL/src/terrain/SkyBox.cpp',

  'OpenGL/src/blocks/Block.cpp',
  'OpenGL/src/blocks/Air_Block.cpp',
  'OpenGL/src/blocks/Dirt_Block.cpp',
  'OpenGL/src/blocks/Stone_Block.cpp',
  'OpenGL/src/blocks/Grass_Block.cpp',
  'OpenGL/src/blocks/Debug_Block.cpp',
)

deps = [
  dependency('glew'),
  dependency('gl'),
  dependency('sdl2'),
  dependency('SDL2_image'),
  dependency('OpenGL'),
  dependency('glfw3'),
  dependency('glm'),
  dependency('threads'),
  dependency('openmp'),
  dependency('sfml-audio'),
]

incs = include_directories('OpenGL/src')

# install_subdir('OpenGL/src/shader', install_dir : 'shader')

exe = executable(
  'MonCraft', srcs,
  dependencies: deps,
  include_directories: incs
)
