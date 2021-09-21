#include "../Hitbox.hpp"

class CharacterHitbox : public Hitbox {
public:
  CharacterHitbox()
    : Hitbox(-glm::vec3(.3f, .5f, .3f), glm::vec3(.3f, 1.3f, .3f))
  { }
};
