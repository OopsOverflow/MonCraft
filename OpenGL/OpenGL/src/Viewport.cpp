#include "Viewport.hpp"

//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// GLEW Libraries
#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

Viewport::Viewport(size_t width, size_t height)
    : camera(width, height, {0, 0, 10}, {0, 0, 0}),
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

  glClearColor(0.0, 0.0, 0.0, 1.0);
  // glEnable(GL_DEPTH_TEST);
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
    turntable.motion(e.motion.x, e.motion.y);
    break;
  case SDL_MOUSEBUTTONDOWN:
    on_mousedown(e.button);
    break;
  case SDL_MOUSEBUTTONUP:
    on_mouseup(e.button);
    break;
  case SDL_MOUSEWHEEL:
    turntable.zoom(e.wheel.y > 0 ? true : false);
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

  fps.apply(camera);
  turntable.apply(camera);

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
    fps.pressedForward();
    break;
  case SDLK_s:
    fps.pressedBackward();
    break;
  case SDLK_d:
    fps.pressedRight();
    break;
  case SDLK_q:
    fps.pressedLeft();
    break;
  case SDLK_SPACE:
    fps.pressedUp();
    break;
  case SDLK_LSHIFT:
    fps.pressedDown();
    break;
  }
}

void Viewport::on_keyup(SDL_Keycode k) {
  switch (k) {
  case SDLK_z:
    fps.releasedForward();
    break;
  case SDLK_s:
    fps.releasedBackward();
    break;
  case SDLK_d:
    fps.releasedRight();
    break;
  case SDLK_q:
    fps.releasedLeft();
    break;
  case SDLK_SPACE:
    fps.releasedUp();
    break;
  case SDLK_LSHIFT:
    fps.releasedDown();
    break;
  }
}

void Viewport::on_mousedown(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
    turntable.rotateStart(e.x, e.y);
    break;
  case SDL_BUTTON_RIGHT:
    turntable.translateStart(e.x, e.y);
    break;
  default:
    break;
  }
}

void Viewport::on_mouseup(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
    turntable.rotateEnd(e.x, e.y);
    break;
  case SDL_BUTTON_RIGHT:
    turntable.translateEnd(e.x, e.y);
    break;
  default:
    break;
  }
}
