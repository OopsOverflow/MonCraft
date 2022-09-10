#pragma once

#include <glm/glm.hpp>

#include "entity/Entity.hpp"
#include "entity/Node.hpp"
#include "terrain/BlockArray.hpp"
#include "util/Raycast.hpp"
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

enum class CharacterView { FIRST_PERSON, THIRD_PERSON, FRONT };

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
	void update(uint32_t dt) override;

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
  void setDab(bool dab);
  bool getDab() const;

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
  bool dab;
};
