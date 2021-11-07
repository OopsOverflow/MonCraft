#pragma once

#include "World.hpp"
#include "gl/Camera.hpp"


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
