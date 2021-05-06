#pragma once

#include <string>

#include "Camera.hpp"
#include "../controller/MouseController.hpp"
#include "../controller/KeyboardController.hpp"

#include <SDL2/SDL.h>

class Viewport
{
public:
  Viewport(size_t width, size_t height);
  ~Viewport();

  static const int framerate; // fps in seconds
  static const int timePerFrame; // time per frame in millis

  bool beginFrame(float& dt);
  void endFrame();

  Camera camera;
  MouseController mouseController;
  KeyboardController keyboardController;

  unsigned width;
  unsigned height;

private:
  void on_event(SDL_Event const& e);
  void on_window_event(SDL_WindowEvent const& e);
  void on_keydown(SDL_Keycode k);
  void on_keyup(SDL_Keycode k);
  void on_mousedown(SDL_MouseButtonEvent const& e);
  void on_mouseup(SDL_MouseButtonEvent const& e);

  SDL_Window* window;
  SDL_GLContext context;

  uint32_t timeBegin;
  uint32_t lastTime;

  bool mouseCaptured = false;
};
