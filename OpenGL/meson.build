project('MonCraft', 'cpp')

srcs = files(
  'OpenGL/src/Application.cpp',
  'OpenGL/src/Shader.cpp',
  'OpenGL/src/Camera.cpp',
  'OpenGL/src/controller/FPS.cpp',
  'OpenGL/src/controller/Turntable.cpp',
)

deps = [
  dependency('glew'),
  dependency('gl'),
  dependency('sdl2'),
  dependency('OpenGL'),
  dependency('glfw3'),
  dependency('glm'),
]

install_subdir('OpenGL/src/shader', install_dir : 'shader')

exe = executable(
  'MonCraft', srcs,
  dependencies : deps
)
