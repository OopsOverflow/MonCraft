#pragma once

#include <memory>

#include "multiplayer/client/Server.hpp"
#include "multiplayer/terrain/TerrainGenerator.hpp"
#include "multiplayer/terrain/PendingChunks.hpp"
#include "util/Identifier.hpp"

class Character;
class World;

class ClientServer : public Server {

public:
  ClientServer();
  virtual ~ClientServer();

  void ping() override;
  void update() override;
  bool login() override;
  std::shared_ptr<Character> getPlayer() override;
  Identifier getUid() override;

private:
  std::shared_ptr<Character> player;
  TerrainGenerator generator;
  PendingChunks pendingChunks;
  World& world;
};
