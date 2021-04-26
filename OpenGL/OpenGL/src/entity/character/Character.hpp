#pragma once

#include "../Entity.hpp"

#include "LeftArm.hpp"
#include "RightArm.hpp"
#include "LeftLeg.hpp"
#include "RightLeg.hpp"
#include "LeftLeg.hpp"
#include "RightLeg.hpp"
#include "CharacterHead.hpp"
#include "CharacterChest.hpp"

class Character : public Entity {

public:
  Character();
  virtual ~Character();

private:
  LeftArm l_arm;
  RightArm r_arm;
  LeftLeg l_leg;
  LeftLeg r_leg;
};
