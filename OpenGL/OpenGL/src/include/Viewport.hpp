#pragma once

#include <string>

#include "Camera.hpp"
#include "controller/Turntable.hpp"
#include "controller/FPS.hpp"

#include <SDL2/SDL.h>

class Viewport
{
public:
  Viewport(size_t width, size_t height);
  ~Viewport();

  bool beginFrame();
  void endFrame();

  Camera camera;
  ControllerTurntable turntable;
  ControllerFPS fps;

  size_t width;
  size_t height;

private:
  void on_event(SDL_Event const& e);
  void on_window_event(SDL_WindowEvent const& e);
  void on_keydown(SDL_Keycode k);
  void on_keyup(SDL_Keycode k);
  void on_mousedown(SDL_MouseButtonEvent const& e);
  void on_mouseup(SDL_MouseButtonEvent const& e);

  SDL_Window* window;
  SDL_GLContext context;
};
