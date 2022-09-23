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
#include "entity/character/Character.hpp"
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
  auto newPlayer = std::make_unique<Character>(Config::getServerConfig().spawnPoint);
  auto entity = World::getInst().entities.add(getUid(), std::move(newPlayer));
  player = std::static_pointer_cast<Character>(entity);
  state = ServerState::CONNECTED;
}

ClientServer::~ClientServer()
{}

void ClientServer::ping() {
  spdlog::info("Clientsidee server ping!");
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
  auto rec = player->popRecord();
  for(auto cpos : rec.getChangedChunks()) {
    auto chunk = world.chunks.find(cpos);
    if(chunk) {
      SaveManager::saveChunk(*chunk);
      
      // need to also save all chunks in which the chunk has generated a slice.
      for(ivec3 cpos : chunk->slices)  {
        auto sliceChunk = world.chunks.find(cpos);
        if(!sliceChunk) {
          spdlog::warn("TODO: chunk {}, containing a slice from {}, is unloaded and cannot be saved.", cpos, chunk->chunkPos);
          continue;
        }
        
        // this is just in case, should not happen, I think.
        auto slices = generator.sliceMap.pop_if(cpos, [cpos = chunk->chunkPos](Structure::Slice const& slice) {
          return slice.origCpos == cpos;
        });
        uint32_t chunkPrio = generator.chunkPriority.sample1D(cpos);
        for(auto const& slice : slices) {
          bool override = generator.chunkPriority.sample1D(slice.origCpos) > chunkPrio;
          Structure::applySlice(*sliceChunk, slice, override);
        }

        spdlog::debug("save slice {}", cpos);
        SaveManager::saveSlice(*sliceChunk);
      }

      // now, we can forget in which chunks the slices have been generated.
      chunk->slices = {};
    }
    else {
      spdlog::warn("Cannot save unloaded chunk: {}", cpos);
    }
  }
}

std::shared_ptr<Character> ClientServer::getPlayer() {
  return player;
}

Identifier ClientServer::getUid() {
  return 0;
}
