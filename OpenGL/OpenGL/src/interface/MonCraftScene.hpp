#pragma once

#include "ui/Component.hpp"

#include "gl/Camera.hpp"
#include "controller/MouseController.hpp"
#include "controller/KeyboardController.hpp"
#include "multiplayer/client/Server.hpp"
#include "audio/Music.hpp"
#include "terrain/World.hpp"
#include "terrain/SkyBox.hpp"
#include "gl/ShadowMap.hpp"
#include "gl/Viewport.hpp"
#include "interface/GameMenu.hpp"
#include "interface/Overlay.hpp"
#include "interface/DebugOverlay.hpp"

class MonCraftScene : public ui::Component {

public:
  MonCraftScene(Viewport* vp);

private:
  void updateShadowMaps();
  void updateUniforms(float t);
  void drawSkybox(float t);
  void drawEntities();
  void draw() override;

protected:
  virtual bool onMousePressed(glm::ivec2 pos) override;
  virtual bool onMouseMove(glm::ivec2 pos) override;
  virtual void onKeyPressed(Key k) override;
  virtual void onKeyReleased(Key k) override;
  virtual bool onActivate() override;

private:
  World& world;
  std::shared_ptr<Character> player;

  // view controls
  Viewport* vp;
  Camera camera;
  KeyboardController keyboardController;
  bool spaceIsPressed;

  // resources
  Shader* shader;
  Shader* fogShader;
  GLuint texAtlas;
  GLuint texCharacter;
  GLuint normalMapID[30];

  //interface
  std::unique_ptr<GameMenu> gameMenu;
  std::unique_ptr<DebugOverlay> debugOverlay;
  std::unique_ptr<Overlay> overlay;
  std::unique_ptr<ui::Image> middleDot;

public:
  // components
  SkyBox sky;
  Raycast caster;
  ShadowMap shadows;
  Music musicPlayer;
  std::shared_ptr<Server> server;

  // other parameters
  bool fogEnabled;
  const float sunSpeed;
  const float skyboxSpeed;
  glm::vec3 sunDir;
  bool captured;
};
