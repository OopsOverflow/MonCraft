#pragma once

#include "../Entity.hpp"
#include "terrain/Terrain.hpp"
#include "terrain/BlockArray.hpp"
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
	void render();

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

  /**
  * Gets the blocks placed since the last call to this method.
  */
  BlockArray getRecord();

  void enableGodMode();
  void disableGodMode();
  void toggleGodMode();
  bool getGodMode();

  void setSprint(bool sprint);

private:
  Node rootNode;
  Head head;
  Chest chest;
  LeftArm l_arm;
  RightArm r_arm;
  LeftLeg l_leg;
  RightLeg r_leg;

  float animState;

  Raycast caster;
  BlockType currentBlock;
  BlockArray record;

  bool god;
  bool sprint;
};
