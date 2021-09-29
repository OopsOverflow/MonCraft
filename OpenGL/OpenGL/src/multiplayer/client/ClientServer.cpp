#include "ClientServer.hpp"
#include <iostream>
#include "multiplayer/common/Config.hpp"
#include "terrain/World.hpp"
#include "entity/character/Character.hpp"
#include "save/SaveManager.hpp"

using namespace glm;

ClientServer::ClientServer()
  : world(World::getInst())
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

void ClientServer::update() {
  if(pendingChunks.changed(player->getPosition())) {
    auto waiting = pendingChunks.get();
    auto count = std::min(waiting.size(), generator.waitingChunks.capacity());
    generator.waitingChunks.clear();
    for (size_t i = 0; i < count; i++) {
      generator.waitingChunks.push(waiting.at(i));
    }
  }
  pendingChunks.remOldChunks();

  // save changes since last update
  auto rec = player->getRecord();
  for(auto const& blockData : rec) {
    ivec3 cpos = floor(vec3(blockData.pos) / float(world.chunkSize));
    auto chunk = world.chunks.find(cpos);
    if(chunk) {
      ivec3 dpos = blockData.pos - cpos * world.chunkSize;
      chunk->setBlock(dpos, AllBlocks::create_static(blockData.type));
      SaveManager::saveChunk(*chunk);
    }
  }
}

bool ClientServer::login() {
  return true;
}

std::shared_ptr<Character> ClientServer::getPlayer() {
  return player;
}
