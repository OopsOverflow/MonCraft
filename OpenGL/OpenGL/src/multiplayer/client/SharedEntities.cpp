#include "SharedEntities.hpp"
#include "../common/Config.hpp"

SharedEntities::SharedEntities()
  : uid(0), player(nullptr), terrain(nullptr)
{ }

entities_ptr_t SharedEntities::create() {
  return std::shared_ptr<SharedEntities>(new SharedEntities());
}

void SharedEntities::createPlayer(Identifier uid) {
  auto res = players.emplace(uid, player_t(new Character(NetworkConfig::SPAWN_POINT)));

  if(!res.second) {
    throw std::runtime_error("Failed to create player: uid already taken");
  }
}

void SharedEntities::initialize() {
  if(terrain || player) {
    throw std::runtime_error("Shared entities already initialized");
  }
  if(!uid) {
    std::cout << "[WARN] entities uid is 0" << std::endl;
  }

  terrain = terrain_t(new Terrain());
  player = player_t(new Character(NetworkConfig::SPAWN_POINT));
}
