#pragma once

#include "ui/Component.hpp"

#include "gl/Camera.hpp"
#include "controller/MouseController.hpp"
#include "controller/KeyboardController.hpp"

#include "audio/Music.hpp"
#include "terrain/Terrain.hpp"
#include "terrain/SkyBox.hpp"
#include "gl/ShadowMap.hpp"
#include "gl/Viewport.hpp"

#include "multiplayer/client/SharedEntities.hpp"

class MonCraftScene : public ui::Component {

public:
  MonCraftScene(Viewport* vp, entities_ptr_t entities);

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
  SkyBox sky;
  Raycast caster;
  ShadowMap shadows;
  Music musicPlayer;
  entities_ptr_t entities;

  // other parameters
  bool fogEnabled;
  const float sunSpeed;
  const float skyboxSpeed;
  glm::vec3 sunDir;
  bool captured;
};
