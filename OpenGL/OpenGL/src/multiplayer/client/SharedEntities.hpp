#pragma once

#include "../common/Identifier.hpp"
#include "entity/character/Character.hpp"
#include <vector>
#include <memory>

struct Player {
  Identifier uid;
  std::unique_ptr<Character> character;
  Player(Identifier uid, glm::vec3 pos);
};

class SharedEntities;
using entities_ptr_t = std::shared_ptr<SharedEntities>;

class SharedEntities {

public:
  Player player;
  std::vector<Player> players;

  static entities_ptr_t create();

private:
  SharedEntities();
};
