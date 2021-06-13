#pragma once

#include "../common/Identifier.hpp"
#include "entity/character/Character.hpp"
#include <vector>
#include <memory>
#include <unordered_map>

class SharedEntities;
using entities_ptr_t = std::shared_ptr<SharedEntities>;

class SharedEntities {
public:
  using player_t = std::unique_ptr<Character>;
  using terrain_t = std::unique_ptr<Terrain>;
  using players_t = std::unordered_map<Identifier, player_t>;

  Identifier uid;
  player_t player;
  players_t players;
  terrain_t terrain;

  void createPlayer(Identifier uid);
  void initialize();

  static entities_ptr_t create();

private:
  SharedEntities();
};
