#pragma once

#include "../Entity.hpp"
#include "gl/Loader.hpp"
#include "terrain/Terrain.hpp"
#include "util/Raycast.hpp"

#include "LeftArm.hpp"
#include "RightArm.hpp"
#include "LeftLeg.hpp"
#include "RightLeg.hpp"
#include "LeftLeg.hpp"
#include "RightLeg.hpp"
#include "Head.hpp"
#include "Chest.hpp"

/**
 * The main character.
 */

class Character : public Entity {

public:

  /**
   * Creates the character at the given position.
   */
  Character(glm::vec3 pos);

  virtual ~Character();

  /**
   * Draws the character entirely, including body parts.
   */
	void render() override;

  /**
   * Update the character state.
   */
	void update(Terrain& terrain, float dt) override;

  /**
   * Breaks the block in line of sight if within reach of the character.
   */
  void breakBlock(Terrain& terrain);

  /**
   * Place a block in front of the player, next to the nearest block face in
   * line of sight, if any.
   */
  void placeBlock(Terrain& terrain);

  /**
   * Choose the block in line of sight to be placed.
   */
  void pickBlock(Terrain& terrain);

private:
  Node rootNode;
  Head head;
  Chest chest;
  LeftArm l_arm;
  RightArm r_arm;
  LeftLeg l_leg;
  RightLeg r_leg;

  GLuint tex;
  Loader texLoader;

  float animState;

  Raycast caster;
  BlockType currentBlock;
};
