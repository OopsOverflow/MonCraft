#include "ClientServer.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "entity/Entities.hpp"
#include "entity/Entity.hpp"
#include "entity/character/CharacterMesh.hpp"
#include "save/SaveManager.hpp"
#include "save/ServerConfig.hpp"
#include "terrain/BlockArray.hpp"
#include "terrain/ChunkMap.hpp"
#include "terrain/World.hpp"
#include "debug/Debug.hpp"

using namespace glm;

ClientServer::ClientServer()
  : world(World::getInst())
{
  auto newPlayer = SaveManager::loadEntity(getUid());
  if (!newPlayer) {
    newPlayer = std::make_unique<CharacterMesh>();
    newPlayer->setPosition(Config::getServerConfig().spawnPoint);
  }
  else {
    spdlog::debug(newPlayer->getPosition());
  }
  auto entity = World::getInst().entities.add(getUid(), std::move(newPlayer));
  player = std::static_pointer_cast<CharacterMesh>(entity);

  { // TODO: this is temporary to debug
    auto newPlayer2 = SaveManager::loadEntity(1);
    if (!newPlayer2) {
      newPlayer2 = std::make_unique<CharacterMesh>();
      newPlayer2->setPosition(Config::getServerConfig().spawnPoint);
    }
    World::getInst().entities.add(1, std::move(newPlayer2));
  }

  state = ServerState::CONNECTED;
  world.t = (uint32_t)(8.f * dayDuration / 24.f);
}

ClientServer::~ClientServer()
{
  world.entities.save();
  world.unload();
}

void ClientServer::ping() {
  spdlog::info("Clientside server ping!");
}

void ClientServer::update() {
  Server::update();

  pendingChunks.update(player->getPosition());
  if(pendingChunks.changed()) {
    auto waiting = pendingChunks.get();
    auto count = std::min(waiting.size(), generator.waitingChunks.capacity());
    generator.waitingChunks.clear();
    for (size_t i = 0; i < count; i++) {
      generator.waitingChunks.push(waiting.at(i));
    }
  }

  // save changes since last update
  auto rec = player->getRecord();
  for(auto cpos : rec.getChangedChunks()) {
    auto chunk = world.chunks.find(cpos);
    if(chunk) {
      SaveManager::saveChunk(*chunk);
    }
  }
}

std::shared_ptr<Entity> ClientServer::getPlayer() {
  return player;
}

Identifier ClientServer::getUid() {
  return 0;
}
