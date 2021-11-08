#pragma once

#include "gl/Camera.hpp"

class World;


class Renderer {

public:
  Renderer();

  /**
  * Renders the visible chunks.
  */
  void render(Camera const& camera);

  /**
  * Renders the visible chunks.
  */
  void renderSolid(Camera const& camera);

private:
  World& world;
};
