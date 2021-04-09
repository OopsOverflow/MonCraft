project('MonCraft', 'cpp')

srcs = files(
  'OpenGL/src/Application.cpp',
  'OpenGL/src/Shader.cpp',
  'OpenGL/src/Camera.cpp',
  'OpenGL/src/Viewport.cpp',

  'OpenGL/src/controller/FPS.cpp',
  'OpenGL/src/controller/Turntable.cpp',

  'OpenGL/src/noise/bitmapTex.cpp',
  'OpenGL/src/noise/value.cpp',
  'OpenGL/src/noise/voronoi.cpp',
  'OpenGL/src/noise/simplex.cpp',
  'OpenGL/src/debug.cpp',
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
]

incs = include_directories('OpenGL/src/include')

# install_subdir('OpenGL/src/shader', install_dir : 'shader')

exe = executable(
  'MonCraft', srcs,
  dependencies: deps,
  include_directories: incs
)
