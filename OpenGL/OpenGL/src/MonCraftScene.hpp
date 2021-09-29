#pragma once

#include "ui/Component.hpp"

#include "gl/Camera.hpp"
#include "controller/MouseController.hpp"
#include "controller/KeyboardController.hpp"

#ifndef EMSCRIPTEN
  #include "audio/Music.hpp"
#endif
#include "terrain/World.hpp"
#include "terrain/SkyBox.hpp"
#include "gl/ShadowMap.hpp"
#include "gl/Viewport.hpp"

class MonCraftScene : public ui::Component {

public:
  MonCraftScene(Viewport* vp, std::shared_ptr<Character> player);

  void drawFrame(float t, float dt);

private:
  void updateShadowMaps();
  void updateUniforms(float t);
  void drawMiddleDot();
  void drawSkybox(float t);
  void drawEntities();

protected:
  virtual bool onMousePressed(glm::ivec2 pos) override;
  virtual bool onMouseMove(glm::ivec2 pos) override;

private:
  World& world;
  std::shared_ptr<Character> player;

  // view controls
  Viewport* vp;
  Camera camera;

  // resources
  Shader* shader;
  Shader* fogShader;
  GLuint texAtlas;
  GLuint texCharacter;
  GLuint normalMapID[30];

  //SaveManager save;

public:
  // components
  SkyBox sky;
  Raycast caster;
  ShadowMap shadows;

  #ifndef EMSCRIPTEN
    Music musicPlayer;
  #endif

  // other parameters
  bool fogEnabled;
  const float sunSpeed;
  const float skyboxSpeed;
  glm::vec3 sunDir;
  bool captured;
};
