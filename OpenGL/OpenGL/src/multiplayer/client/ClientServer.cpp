#include "ClientServer.hpp"
#include <iostream>
#include "multiplayer/common/Config.hpp"
#include "terrain/World.hpp"
#include "entity/character/Character.hpp"

ClientServer::ClientServer()
{
  auto newPlayer = std::make_unique<Character>(NetworkConfig::SPAWN_POINT);
  auto entity = World::getInst().entities.add(0, std::move(newPlayer));
  player = std::dynamic_pointer_cast<Character>(entity);
}

ClientServer::~ClientServer()
{}

void ClientServer::ping() {
  std::cout << "clientside server ping!" << std::endl;
}

void ClientServer::update()
{}

std::shared_ptr<Character> ClientServer::getPlayer() {
  return player;
}
