#pragma once
#include "Server.hpp"
#include "multiplayer/terrain/TerrainGenerator.hpp"
#include "multiplayer/terrain/PendingChunks.hpp"

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
