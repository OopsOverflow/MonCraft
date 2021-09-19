#pragma once

#include "ui/Component.hpp"

#include "gl/Camera.hpp"
#include "controller/MouseController.hpp"
#include "controller/KeyboardController.hpp"

#ifndef EMSCRIPTEN
#include "audio/Music.hpp"
#endif
#include "terrain/Terrain.hpp"
#include "terrain/SkyBox.hpp"
#include "gl/ShadowMap.hpp"
#include "gl/Viewport.hpp"

class MonCraftScene : public ui::Component {

public:
  MonCraftScene(Viewport* vp);

  void drawFrame(float t, float dt);

private:
  void updateShadowMaps();
  void updateUniforms(float t);
  void drawMiddleDot();
  void drawSkybox(float t);
  void drawTerrain();
  void drawCharacter();

protected:
  virtual bool onMousePressed(glm::ivec2 pos) override;
  virtual bool onMouseMove(glm::ivec2 pos) override;

private:
  // view controls
  Viewport* vp;
  Camera camera;

  // resources
  Shader* shader;
  Shader* fogShader;
  GLuint texAtlas;
  GLuint texCharacter;
  GLuint normalMapID[30];

public:
  // components
  Terrain terrain;
  SkyBox sky;
  Raycast caster;
  ShadowMap shadows;
  Character character;
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
