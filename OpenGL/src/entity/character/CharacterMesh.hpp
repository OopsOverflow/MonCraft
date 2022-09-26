#pragma once

#include "Character.hpp"

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

class CharacterMesh : public Character {

public:

  /**
   * Creates the character at the given position.
   */
  CharacterMesh();

  virtual ~CharacterMesh();

  /**
   * Update the character state.
   */
	void update(uint32_t dt) override;

  /**
   * Draws the character entirely, including body parts.
   */
	void render() override;

private:
  Head head;
  Chest chest;
  LeftArm l_arm;
  RightArm r_arm;
  LeftLeg l_leg;
  RightLeg r_leg;

};
