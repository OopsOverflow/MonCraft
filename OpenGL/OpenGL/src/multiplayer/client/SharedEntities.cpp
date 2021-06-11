#include "SharedEntities.hpp"
#include "../common/Config.hpp"

Player::Player(Identifier uid, glm::vec3 pos)
  : uid(uid), character(new Character(pos))
{}

SharedEntities::SharedEntities()
  : player(0, NetworkConfig::SPAWN_POINT)
{ }

entities_ptr_t SharedEntities::create() {
  return std::shared_ptr<SharedEntities>(new SharedEntities());
}
