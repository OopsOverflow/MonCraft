#pragma once

#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>

#include "ui/Component.hpp"
#include "gl/Camera.hpp"
#include "controller/KeyboardController.hpp"
#include "ui/Image.hpp"
#include "ui/Key.hpp"
#include "util/Identifier.hpp"
#include "util/Raycast.hpp"

#ifndef EMSCRIPTEN
  #include "audio/Music.hpp"
#endif

#include "gl/SkyBox.hpp"
#include "gl/ShadowMap.hpp"
#include "terrain/Renderer.hpp"
#include "interface/GameMenu.hpp"
#include "interface/Overlay.hpp"
#include "interface/DebugOverlay.hpp"

class Character;
class Server;
class Shader;
class Viewport;
class World;
namespace Config { struct ClientConfig; }

class MonCraftScene : public ui::Component {

public:
  MonCraftScene(Viewport* vp);

private:
  void updateFov(uint32_t dt);
  void updateShadowMaps();
  void updateUniforms(uint32_t t);
  void drawSkybox();
  void drawEntities();
  void draw() override;

protected:
  virtual bool onMousePressed(glm::ivec2 pos) override;
  virtual bool onMouseMove(glm::ivec2 pos) override;
  virtual void onKeyPressed(Key k) override;
  virtual void onKeyReleased(Key k) override;

private:
  World& world;
  Config::ClientConfig& config;
  Renderer renderer;
  std::shared_ptr<Character> player;
  Identifier playerUid;

  // view controls
  Viewport* vp;
  Camera camera;
  KeyboardController keyboardController;

  // resources
  Shader* shader;
  GLuint texAtlas;
  GLuint texCharacter;
  GLuint normalMapID[30];

  //interface
  std::unique_ptr<DebugOverlay> debugOverlay;
  std::unique_ptr<Overlay> overlay;
  std::unique_ptr<ui::Image> middleDot;

public:
  std::unique_ptr<GameMenu> gameMenu;

  // components
  SkyBox sky;
  Raycast caster;
  ShadowMap shadows;
  std::shared_ptr<Server> server;

  #ifndef EMSCRIPTEN
    Music musicPlayer;
  #endif

  // other parameters
  bool fogEnabled;
  const float sunSpeed;
  glm::vec3 sunDir;
};
