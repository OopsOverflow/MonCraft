#pragma once

#include <glm/glm.hpp>
#include <vector>

enum class Projection {
  PROJECTION_ORTHOGRAPHIC,
  PROJECTION_PERSPECTIVE,
  CUSTOM_PROJECTION };

enum class Frustum { NEAR = 0, MEDIUM = 1, FAR = 2, ALL = 3 };

/**
 * Basically manages a view matrix.
 */

class Camera {
public:

  /**
   * Constructs a camera with a given size, lookat (position/center) and
   * projection type.
   */
  Camera(glm::ivec2 size, const glm::vec3& position,
      const glm::vec3& center, Projection proj = Projection::PROJECTION_PERSPECTIVE);

  /**
   * Activates the camera.
   * In a nutshell, binds its view matrix to the currently activated shader.
   */
  void activate();

  /**
   * Sets the camera width / height and updates itself accordingly.
   */
  void setSize(glm::ivec2 size);

  /**
   * Switches smoothly between different projection types.
   */
  void setProjectionType(Projection projType);
  void setProjectionType(Projection projType, float box[6]);

  /**
   * Sets the camera position and updates itself accordingly.
   */
  void setPosition(const glm::vec3 &position);

  /**
   * Sets the camera lookat and updates itself accordingly.
   */
  void setLookAt(const glm::vec3 &pos, const glm::vec3 &center);

  /**
   * Translate the camera by an amount in world space.
   * Can translate in global space or camera space.
   */
  void translate(const glm::vec3 &direction, bool localSpace = false);

  /**
   * Translate the camera by an amount given in pixels unit.
   */
  void translatePixels(int x, int y);

  /**
   * Rotate the camera along the 3 axes in local space or world space.
   * The vector rotation gives the rotaton amount around its corresponding
   * axis, in degrees.
   */
  void rotate(const glm::vec3 &rotation, bool localSpace = false);

  /**
   * Rotates the camera along the by an amount given in pixels unit.
   * Can rotate in global space or camera space.
   */
  void rotatePixels(int x, int y, bool localSpace = false);
  // void setRotation(const glm::vec3 &rotation);

  /**
   * Sets the vertical fov and updates itself accordingly.
   */
  void setFovY(float fovY);

  /**
   * Gets the vertical fov.
   */
  float getFovY() const;

  /**
   * Gets the horizontal fov.
   */
  float getFovX() const;

  /**
   * Gets the camera dimensions.
   * Copies the camera dimensions in width and height.
   */
  glm::ivec2 getSize() const;

  /**
   * Gets the projection type.
   */
  Projection getProjectionType() const;

  /**
   * Gets the coordinates of the 8 corners of the view frustum,
   * in world coordinates. The Frustum provided corresponds to a third of the
   * frustum length. Frustum::ALL actually corresponds to the whole frustum.
   */
  std::vector<glm::vec3> getBoxCorners(Frustum frustum = Frustum::ALL) const;

  bool chunkInView(glm::vec3 posCamSpace, float tolerance) const;

public:
  glm::mat4 view;
  glm::mat4 projection;
  glm::vec3 position;
  glm::vec3 center;
  float near_;
  float far_;

protected:
  float fovY;
  glm::ivec2 size;
  Projection projType;

private:
    float tanFovY;
    float tanFovX;
    float custumProjBox[6];


  void computeProjection(float box[6]);
  void computeProjection();
  void computeView();
};
