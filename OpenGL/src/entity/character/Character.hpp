#pragma once

#include <glm/glm.hpp>

#include "entity/Entity.hpp"
#include "entity/Node.hpp"
#include "terrain/BlockArray.hpp"
#include "blocks/Block.hpp"

#include "LeftArm.hpp"
#include "RightArm.hpp"
#include "LeftLeg.hpp"
#include "RightLeg.hpp"
#include "Head.hpp"
#include "Chest.hpp"

class Camera;

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
	void update(uint32_t dt) override;

  /**
   * Breaks the block in line of sight if within reach of the character.
   */
  void leftClick();

  /**
   * Place a block in front of the player, next to the nearest block face in
   * line of sight, if any.
   */
  void rightClick();

  /**
   * Choose the block in line of sight to be placed.
   */
  void middleClick();

  /**
  * Gets the blocks placed since the last call to this method.
  */
  BlockArray& getRecord();

  /**
   * Set the selected block type (in hand).
   */
  void setCurrentBlock(BlockType type);

  /**
   * Return the selected block type (in hand).
   */
  BlockType getCurrentBlock() const;

private:
  Node rootNode;
  Head head;
  Chest chest;
  LeftArm l_arm;
  RightArm r_arm;
  LeftLeg l_leg;
  RightLeg r_leg;

  float animState;

  BlockType currentBlock;
  BlockArray record;

};
