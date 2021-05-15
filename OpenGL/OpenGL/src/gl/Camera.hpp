#pragma once

#include <glm/glm.hpp>
#include <vector>

enum class Projection {
  PROJECTION_ORTHOGRAPHIC,
  PROJECTION_PERSPECTIVE,
  CUSTOM_PROJECTION };

enum class Frustum { NEAR = 0, MEDIUM = 1, FAR = 2, ALL = 3 };

class Camera {
public:
    Camera(unsigned int width, unsigned int height, const glm::vec3& position,
        const glm::vec3& center, Projection proj = Projection::PROJECTION_PERSPECTIVE);

  void activate();

  void setSize(unsigned int width, unsigned int height);
  void setProjectionType(Projection projType);
  void setProjectionType(Projection projType, float box[6]);
  void setPosition(const glm::vec3 &position);
  void setLookAt(const glm::vec3 &pos, const glm::vec3 &center);

  void translate(const glm::vec3 &direction, bool localSpace = false);
  void translatePixels(int x, int y);

  void rotate(const glm::vec3 &rotation, bool localSpace = false);
  void rotatePixels(int x, int y, bool localSpace = false);
  // void setRotation(const glm::vec3 &rotation);

  void setFovY(float fovY) { this->fovY = fovY; }
  float getFovY() const { return fovY; }
  float getFovX() const { return glm::degrees(2 * atan(tan(glm::radians(fovY) * 0.5) * screenWidth / screenHeight)); } // see https://en.wikipedia.org/wiki/Field_of_view_in_video_games#Field_of_view_calculations

  void getSize(unsigned int &width, unsigned int &height) const;
  Projection getProjectionType() const;

  std::vector<glm::vec3> getBoxCorners(Frustum frustum) const;

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
  unsigned int screenWidth;
  unsigned int screenHeight;
  Projection projType;

private:
    float tanFovY;
    float tanFovX;
    float custumProjBox[6];


  void computeProjection(float box[6]);
  void computeProjection();
  void computeView();
};
