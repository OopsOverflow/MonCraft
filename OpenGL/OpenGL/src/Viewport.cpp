#include "Viewport.hpp"

//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// GLEW Libraries
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

Viewport::Viewport(size_t width, size_t height)
    : camera(width, height, {0, 64, 10}, {0, 64, 20}),
      width(width),
      height(height),
      window(nullptr),
      context(nullptr)
{

  //Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
      throw std::runtime_error(std::string("SDL init failed: ") + SDL_GetError());
  }

  //Create a Window
  window = SDL_CreateWindow("MonCraft",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  //Initialize the OpenGL Context
  context = SDL_GL_CreateContext(window);

  //Check GLEW Intitialisation
  if (glewInit() != GLEW_OK)
    throw std::runtime_error("GLEW init failed");

  std::cout << "---------" << std::endl;
  std::cout << "OpenGL  :" << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL    :" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor  :" << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer:" << glGetString(GL_RENDERER) << std::endl;
  std::cout << "---------" << std::endl;

  glClearColor(54/255.f, 199/255.f, 242/255.f, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

Viewport::~Viewport() {
    //Free everything
    if (context)
        SDL_GL_DeleteContext(context);
    if (window)
        SDL_DestroyWindow(window);
}

void Viewport::on_event(SDL_Event const& e) {
  switch (e.type) {
  case SDL_WINDOWEVENT:
    on_window_event(e.window);
    break;
  case SDL_KEYDOWN:
    on_keydown(e.key.keysym.sym);
    break;
  case SDL_KEYUP:
    on_keyup(e.key.keysym.sym);
    break;
  case SDL_MOUSEMOTION:
    mouseController.motion(e.motion.x, e.motion.y);
    break;
  case SDL_MOUSEBUTTONDOWN:
    on_mousedown(e.button);
    break;
  case SDL_MOUSEBUTTONUP:
    on_mouseup(e.button);
    break;
  }
}

bool Viewport::beginFrame() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if(event.type == SDL_WINDOWEVENT)
      if(event.window.event == SDL_WINDOWEVENT_CLOSE)
        return false;
    on_event(event);
  }
  return true;
}

void Viewport::endFrame() {
  //Display on screen (swap the buffer on screen and the buffer you are drawing on)
  SDL_GL_SwapWindow(window);
}

void Viewport::on_window_event(SDL_WindowEvent const& e) {
  switch (e.event) {
  case SDL_WINDOWEVENT_SIZE_CHANGED:
    width = e.data1;
    height = e.data2;
    camera.setSize(width, height);
    break;
  }
}

void Viewport::on_keydown(SDL_Keycode k) {
  switch (k) {
  case SDLK_z:
    keyboardController.pressedForward();
    break;
  case SDLK_s:
    keyboardController.pressedBackward();
    break;
  case SDLK_d:
    keyboardController.pressedRight();
    break;
  case SDLK_q:
    keyboardController.pressedLeft();
    break;
  case SDLK_SPACE:
    keyboardController.pressedUp();
    break;
  case SDLK_LSHIFT:
    keyboardController.pressedDown();
    break;
  case SDLK_F5:
      keyboardController.pressedF5();
      break;
  }
}

void Viewport::on_keyup(SDL_Keycode k) {
  switch (k) {
  case SDLK_z:
    keyboardController.releasedForward();
    break;
  case SDLK_s:
    keyboardController.releasedBackward();
    break;
  case SDLK_d:
    keyboardController.releasedRight();
    break;
  case SDLK_q:
    keyboardController.releasedLeft();
    break;
  case SDLK_SPACE:
    keyboardController.releasedUp();
    break;
  case SDLK_LSHIFT:
    keyboardController.releasedDown();
    break;
  }
}

void Viewport::on_mousedown(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
      mouseController.rotateStart(e.x, e.y);
    break;
  default:
    break;
  }
}

void Viewport::on_mouseup(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
      mouseController.rotateEnd(e.x, e.y);
    break;
  default:
    break;
  }
}
