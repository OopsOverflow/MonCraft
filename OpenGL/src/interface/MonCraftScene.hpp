#pragma once

#include <GL/glew.h>
#include <memory>
#include <glm/glm.hpp>

#include "ui/Component.hpp"
#include "gl/Camera.hpp"
#include "controller/PlayerController.hpp"
#include "ui/Image.hpp"
#include "ui/Key.hpp"
#include "util/Identifier.hpp"
#include "util/Raycast.hpp"
#include "parametersMenu/ParametersMenu.hpp"

#ifndef EMSCRIPTEN
  #include "audio/Music.hpp"
  #include "gl/ShadowMap.hpp"
#endif

#include "gl/SkyBox.hpp"
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
  static std::unique_ptr<MonCraftScene> create(Viewport* vp);
  
private:
  MonCraftScene(Viewport* vp);
  void updateFov(uint32_t dt);
  #ifndef EMSCRIPTEN
    void updateShadowMaps();
  #endif
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
  Identifier playerUid;

  // view controls
  Viewport* vp;
  Camera camera;
  std::unique_ptr<PlayerController> playerController;

  // resources
  Shader* shader;
  GLuint texAtlas;
  GLuint texCharacter;
  GLuint normalMapID[30];

  //interface
  std::shared_ptr<DebugOverlay> debugOverlay;  
  std::shared_ptr<ui::Image> middleDot;

  bool debug;

public:
  std::shared_ptr<GameMenu> gameMenu;
  std::shared_ptr<Overlay> overlay;
  std::shared_ptr<ParametersMenu> parameters;

  // components
  SkyBox sky;
  std::shared_ptr<Server> server;

  #ifndef EMSCRIPTEN
    ShadowMap shadows;
    Music musicPlayer;
  #endif

  // other parameters
  const float sunSpeed;
  glm::vec3 sunDir;

  uint32_t lastClock;
};
