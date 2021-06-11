#include "SharedEntities.hpp"
#include "../common/Config.hpp"

SharedEntities::SharedEntities()
  : player(nullptr)
{ }

entities_ptr_t SharedEntities::create() {
  return std::shared_ptr<SharedEntities>(new SharedEntities());
}

SharedEntities::player_t SharedEntities::createPlayer() {
  return player_t(new Character(NetworkConfig::SPAWN_POINT));
}
