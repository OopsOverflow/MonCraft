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

  Spline breakAnim;
  Spline rightArmWalkAnim;
  Spline leftArmWalkAnim;
  Spline rightLegWalkAnim;
  Spline leftLegWalkAnim;

  bool god;
  bool sprint;
};

static const std::vector<std::pair<float, glm::vec3> > breakKeyframes = {
  {0.f,       {-0.50f, -0.25f, 0.80f}},
  {1.f/ 12.f, {-0.65f, 0.05f, 0.80f}},
  {2.f/ 12.f, {0.15f, 0.25f, 1.f}},
  {3.f/ 12.f, {0.15f, -0.25f, 1.f}},
  
};

static const std::vector<std::pair<float, glm::vec3> > rightArmWalkKeyframes = {
  {0.f,       {0.f, -1.f, 1.0f}},
  {1.f / 4.f, {0.05f, -1.f, 0.f}},
  {2.f / 4.f, {0.f, -1.f, -1.0f}},
  {3.f / 4.f, {-0.05f, -1.f, 0.f}},
  {1.f,       {0.f, -1.f, 1.0f}},
  
};

static const std::vector<std::pair<float, glm::vec3> > leftArmWalkKeyframes = {
  {0.f,       {0.f, -1.f, -1.0f}},
  {1.f / 4.f, {-0.05f, -1.f, 0.f}},
  {2.f / 4.f, {0.f, -1.f, 1.0f}},
  {3.f / 4.f, {0.05f, -1.f, 0.f}},
  {1.f,       {0.f, -1.f, -1.0f}},
  
};

static const std::vector<std::pair<float, glm::vec3> > leftLegWalkKeyframes = {
  {0.f,       {0.f, -1.f, 1.0f}},
  {1.f / 4.f, {0.01f, -1.f, 0.f}},
  {2.f / 4.f, {0.f, -1.f, -1.0f}},
  {3.f / 4.f, {-0.01f, -1.f, 0.f}},
  {1.f,       {0.f, -1.f, 1.0f}},
  
};

static const std::vector<std::pair<float, glm::vec3> > rightLegWalkKeyframes = {
  {0.f,       {0.f, -1.f, -1.0f}},
  {1.f / 4.f, {-0.01f, -1.f, 0.f}},
  {2.f / 4.f, {0.f, -1.f, 1.0f}},
  {3.f / 4.f, {0.01f, -1.f, 0.f}},
  {1.f,       {0.f, -1.f, -1.0f}},
  
};