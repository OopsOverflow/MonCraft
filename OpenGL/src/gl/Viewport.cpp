#include "Viewport.hpp"
#include <spdlog/spdlog.h>

// send a esc keypress on pointerlock leave.
#ifdef EMSCRIPTEN
  EM_BOOL onPointerLockChange(int evtType, EmscriptenPointerlockChangeEvent const* evt, void* data) {
    Viewport* vp = (Viewport*)data;
  
    if (!evt->isActive) {
      SDL_Event evt {
        .key = {
          .type = SDL_KEYUP,
          .timestamp = 0,
          .windowID = 0,
          .state = SDL_RELEASED,
          .repeat = 0,
          .keysym = {
            .scancode = SDL_SCANCODE_ESCAPE,
            .sym = SDLK_ESCAPE,
            .mod = 0,
          }
        }
      };
      vp->on_event(evt);
    }
    return false;
  }
#endif

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <glm/glm.hpp>
#include <algorithm>

#include "ui/Event.hpp"
#include "ui/Root.hpp"

const int Viewport::framerate = 60;
const int Viewport::timePerFrame = 1000 / framerate;

// force use of dedicated gpu on Nvidia optimus laptops / Amd laptops
#ifdef _WIN32
#include <windows.h>
extern "C" {
  __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#undef min
#endif



Viewport::Viewport(glm::ivec2 size)
  :   window(nullptr), context(nullptr),
      mouseCaptured(false), mustQuit(false),
      root(nullptr), mouseScroll(0)
{
  // Initialize SDL2
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    throw std::runtime_error(std::string("SDL init failed: ") + SDL_GetError());

  SDL_DisplayMode dm;
  if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
      throw std::runtime_error(std::string("SDL_GetDesktopDisplayMode failed: ") + SDL_GetError());

  // MSAA
  if(Config::getClientConfig().msaa) {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, Config::getClientConfig().msaa);
  }

  size.x = std::min((int)size.x, (int)(dm.w * 0.9));
  size.y = std::min((int)size.y, (int)(dm.h * 0.9));
  this->size = size;

  //Create a Window
  window = SDL_CreateWindow("MonCraft",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      size.x, size.y,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  toggleFullscreen();
  toggleVSync();

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  #ifdef EMSCRIPTEN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    // TODO
    // EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_get_current_context();
    // EmscriptenWebGLContextAttributes attrs;
    // emscripten_webgl_get_context_attributes(ctx, &attrs);
    // emscripten_webgl_enable_extension(ctx, "WEBGL_depth_texture");

    // handle esc keypress when pointerlock is enabled
    EMSCRIPTEN_RESULT res = emscripten_set_pointerlockchange_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, &onPointerLockChange);
    if (res < 0)
      std::cerr << "Failed to watch pointerlock change event in emscripten" << std::endl;
  #endif

  //Initialize the OpenGL Context
  context = SDL_GL_CreateContext(window);

  // Initialize GLEW
  if (glewInit() != GLEW_OK)
    throw std::runtime_error("GLEW init failed");

  spdlog::info("---------");
  spdlog::info("OpenGL  : {}", glGetString(GL_VERSION));
  spdlog::info("GLSL    : {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
  spdlog::info("Vendor  : {}", glGetString(GL_VENDOR));
  spdlog::info("Renderer: {}", glGetString(GL_RENDERER));
  spdlog::info("---------");
}

void Viewport::createRoot() {
  root = std::make_unique<ui::Root>(size);
}

Viewport::~Viewport() {
    //Free everything
    if (context)
        SDL_GL_DeleteContext(context);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
  
  #ifdef EMSCRIPTEN
    // disable esc keypress handling
    emscripten_set_pointerlockchange_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, this, true, nullptr);
  #endif
}

void Viewport::quit() {
  mustQuit = true;
}

ui::Root* Viewport::getRoot() {
  return root.get();
}

void Viewport::captureMouse() {
  SDL_SetRelativeMouseMode(SDL_TRUE);
  mouseCaptured = true;
  int x, y;
  SDL_GetMouseState(&x, &y);
  mouseController.rotateStart(x, y);
}

void Viewport::freeMouse() {
  SDL_SetRelativeMouseMode(SDL_FALSE);
  mouseCaptured = false;
  int x, y;
  SDL_GetMouseState(&x, &y);
  mouseController.rotateEnd(x, y);
}

void Viewport::toggleVSync() {
  if(Config::getClientConfig().vsync) SDL_GL_SetSwapInterval(1);
  else SDL_GL_SetSwapInterval(0);
}

void Viewport::toggleFullscreen() {
  if(Config::getClientConfig().fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  else SDL_SetWindowFullscreen(window, 0);
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
    if(mouseCaptured) {
      mouseController.motionRel(e.motion.xrel, e.motion.yrel);
    }
    else {
      root->addEvent(Event(Event::Type::MOVE, {e.motion.x, size.y - e.motion.y}));
    }
    break;
  case SDL_MOUSEBUTTONDOWN:
    on_mousedown(e.button);
    break;
  case SDL_MOUSEBUTTONUP:
    on_mouseup(e.button);
    break;
  case SDL_MOUSEWHEEL:
    on_mouse_scroll(e.wheel);
    break;
  }
}

bool Viewport::beginFrame() {
  if(mustQuit) return false;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if(event.type == SDL_WINDOWEVENT)
      if(event.window.event == SDL_WINDOWEVENT_CLOSE)
        return false;
    if(event.type == SDL_QUIT)
      return false;
    on_event(event);
  }
  
  glViewport(0, 0, size.x, size.y);
  glClearColor(255/255.f, 0/255.f, 203/255.f, 1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  #ifndef EMSCRIPTEN
    glEnable(GL_MULTISAMPLE);
  #endif
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return true;
}

void Viewport::endFrame() {
  root->update();
  root->render();

  //Display on screen (swap the buffer on screen and the buffer you are drawing on)
  SDL_GL_SwapWindow(window);
  
}

void Viewport::on_window_event(SDL_WindowEvent const& e) {
  switch (e.event) {
  case SDL_WINDOWEVENT_SIZE_CHANGED:
      size.x = e.data1;
      size.y = e.data2;
      root->setSize(size);
      break;
  }
}

void Viewport::on_keydown(SDL_Keycode k) {
  root->keyPress(k); // controllers in ui (MonCraftScene)
}

void Viewport::on_keyup(SDL_Keycode k) {
  root->keyRelease(k); // controllers in ui (MonCraftScene)
}

void Viewport::on_mousedown(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
    if(mouseCaptured) {
      mouseController.triggerAction(MouseController::Action::DESTROY);
    }
    else {
      root->addEvent(Event(Event::Type::PRESS, {e.x, size.y - e.y}));
    }
    break;
  case SDL_BUTTON_RIGHT:
    if(mouseCaptured) {
      mouseController.triggerAction(MouseController::Action::PLACE);
    }
    break;
  case SDL_BUTTON_MIDDLE:
    if(mouseCaptured) {
      mouseController.triggerAction(MouseController::Action::PICK);
    }
    break;
  default:
    break;
  }
}

void Viewport::on_mouseup(SDL_MouseButtonEvent const& e) {
  switch (e.button) {
  case SDL_BUTTON_LEFT:
      if(!mouseCaptured) {
        root->addEvent(Event(Event::Type::RELEASE, {e.x, size.y - e.y}));
      }
    break;
  default:
    break;
  }
}

void Viewport::on_mouse_scroll(SDL_MouseWheelEvent const& e) {
  if(e.y > 0) {
    if(mouseCaptured)
      mouseScroll -= 1;
  }else if(e.y < 0) {
    if(mouseCaptured)
      mouseScroll +=1;
  }
}

int Viewport::getMouseScrollDiff() {
  auto tmp = mouseScroll;
  mouseScroll = 0;
  return tmp;
}