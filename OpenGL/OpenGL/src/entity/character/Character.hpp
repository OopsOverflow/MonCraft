#pragma once

#include "gl/Camera.hpp"
#include "../Entity.hpp"
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

enum class CharacterView { FIRST_PERSON, THIRD_PERSON };

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

  void cameraToHead(Camera& camera);

  /**
   * Update the character state.
   */
	void update(float dt) override;

  /**
   * Breaks the block in line of sight if within reach of the character.
   */
  void breakBlock();

  /**
   * Place a block in front of the player, next to the nearest block face in
   * line of sight, if any.
   */
  void placeBlock();

  /**
   * Choose the block in line of sight to be placed.
   */
  void pickBlock();

  /**
  * Gets the blocks placed since the last call to this method.
  */
  BlockArray& getRecord();

  void enableGodMode();
  void disableGodMode();
  void toggleGodMode();
  bool getGodMode();

  void setSprint(bool sprint);

  /**
   * Set the selected block type (in hand).
   */
  void setCurrentBlock(BlockType type);

  /**
   * Return the selected block type (in hand).
   */
  BlockType getCurrentBlock() const;

  CharacterView view;

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
