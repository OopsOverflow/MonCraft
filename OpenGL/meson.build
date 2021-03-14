project('MonCraft', 'cpp')

srcs = files(
  'OpenGL/src/Application.cpp'
)

deps = [
  dependency('glew'),
  dependency('gl'),
  dependency('sdl2'),
  dependency('OpenGL'),
]

exe = executable(
  'MonCraft', srcs,
  dependencies : deps
)
