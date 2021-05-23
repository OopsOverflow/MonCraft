#pragma once

#include "../gl/Shader.hpp"
#include "../gl/Camera.hpp"

/**
 * A cascading shadow map.
 * 3 shadow textures are creadted : near, middle and far.
 */

class ShadowMap {

public:
  /**
   * Constructs a shadowmap with the given resolution.
   */
  ShadowMap(int size);
  ~ShadowMap();

  ShadowMap(ShadowMap const&) = delete;
  ShadowMap& operator=(ShadowMap&) = delete;

  /**
   * Updates the shadow matrices using a sun direction.
   */
  void update(glm::vec3 sunDir);

  /**
   * Updates the shadow matrices using a sun direction.
   */
  void attach(Camera const& cam, Frustum frustum);

  /**
   * Render of a given view frustum must be done between a beginFrame() /
   * endframe().
   */
  void beginFrame(Frustum frustum);
  void endFrame();

  /**
   * Activates the shadowmap to render shadows.
   * basically binds the shadow matrices to the activated shader.
   */
  void activate();

  /**
   * Gets the texture associated to a given frustum - near, middle or far.
   */
  GLuint getTextureID(Frustum frustum) const;

  /**
   * The shadow camera. /!\ this is readonly.
   */
  Camera camera;

  /**
   * The shadow matrices. /!\ this is readonly.
   */
  glm::mat4 shadowMatrices[3];
private:
  GLuint fbo;
  GLuint depthTex[3];
  float resolution;
  Shader shader;
  float distance;
  int size;
};
