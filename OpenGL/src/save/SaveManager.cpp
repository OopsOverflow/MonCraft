#include "SaveManager.hpp"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <stdint.h>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "blocks/AllBlocks.hpp"
#include "entity/Node.hpp"
#include "terrain/ChunkImpl.hpp"
#include "util/Serde.hpp"
#include "util/zstr.hpp"

using serde::Binary;

std::string SaveManager::chunkSaveDir = "save/defaultWorld/chunks";
std::string SaveManager::entitySaveDir = "save/defaultWorld/entities";
std::string SaveManager::configSaveDir = "save";

std::ostream& serde::operator<<(std::ostream &stream, const glm::vec3 &vec) {
  Binary<float> x = vec.x, y = vec.y, z = vec.z;
  stream << x << y << z;
  return stream;
}
std::istream& serde::operator>>(std::istream &stream, glm::vec3 &vec) {
  Binary<float> x, y, z;
  stream >> x >> y >> z;
  vec.x = x.val;
  vec.y = y.val;
  vec.z = z.val;
  return stream;
}

std::ostream& serde::operator<<(std::ostream &stream, State state) {
  Binary temp((uint8_t)state);
  stream << temp;
  return stream;
}
std::istream& serde::operator>>(std::istream &stream, State &state) {
  Binary<uint8_t> temp;
  stream >> temp;
  state = (State)temp.val;
  return stream;
}

std::ostream& serde::operator<<(std::ostream &stream, Facing facing) {
  Binary temp((uint8_t)facing);
  stream << temp;
  return stream;
}
std::istream& serde::operator>>(std::istream &stream, Facing &facing) {
  Binary<uint8_t> temp;
  stream >> temp;
  facing = (Facing)temp.val;
  return stream;
}

std::ostream& serde::operator<<(std::ostream &stream, EntityClass entityClass) {
  Binary temp((uint8_t)entityClass);
  stream << temp;
  return stream;
}
std::istream& serde::operator>>(std::istream &stream, EntityClass &entityClass) {
  Binary<uint8_t> temp;
  stream >> temp;
  entityClass = (EntityClass)temp.val;
  return stream;
}

std::ostream& serde::operator<<(std::ostream &stream, const Node &node) {
  stream << node.loc << node.rot << node.sca;
  return stream;
}
std::istream& serde::operator>>(std::istream &stream, Node &node) {
  glm::vec3 loc, rot, sca;

  stream >> loc >> rot >> sca;
  node.loc = loc;
  node.rot = rot;
  node.sca = sca;
  return stream;
}

std::ostream& serde::operator<<(std::ostream &stream, const Entity &entity) {
  stream << entity.bodyNode << entity.headNode << entity.state;
  return stream;
}
std::istream& serde::operator>>(std::istream &stream, Entity &entity) {
  Node head, node;
  stream >> entity.bodyNode >> entity.headNode >> entity.state;
  return stream;
}

std::ostream& serde::operator<<(std::ostream &stream, BlockType type) {
  Binary bin((uint8_t)type);
  stream << bin;
  return stream;
}
std::istream& serde::operator>>(std::istream &stream, BlockType &type) {
  Binary<uint8_t> temp;
  stream >> temp;
  type = (BlockType)temp.val;
  return stream;
}

std::istream& serde::operator>>(std::istream &stream, ChunkImpl &chunk) {
  glm::vec3 pos(0);
  for (pos.y = 0; pos.y < chunk.size().y; pos.y++) {
    for (pos.z = 0; pos.z < chunk.size().z; pos.z++) {
      for (pos.x = 0; pos.x < chunk.size().x; pos.x++) {
        chunk.at(pos) = AllBlocks::deserialize(stream);
      }
    }
  }

  return stream;
}
std::ostream& serde::operator<<(std::ostream &stream, ChunkImpl const &chunk) {
  glm::vec3 pos(0);
  for (pos.y = 0; pos.y < chunk.size().y; pos.y++) {
    for (pos.z = 0; pos.z < chunk.size().z; pos.z++) {
      for (pos.x = 0; pos.x < chunk.size().x; pos.x++) {
        AllBlocks::serialize(stream, chunk.at(pos).get());
      }
    }
  }

  return stream;
}

std::unique_ptr<ChunkImpl> SaveManager::loadChunk(glm::ivec3 chunkPos) {
  using namespace serde;
  std::string filePath = chunkSaveDir + "/chunk_" +
                         std::to_string(chunkPos.x) + "_" +
                         std::to_string(chunkPos.y) + "_" +
                         std::to_string(chunkPos.z) + ".chunk";

  zstr::ifstream openedFile(filePath, std::fstream::binary);
  if (!openedFile.is_open())
    return nullptr;

  Binary<uint8_t> chunkSize;
  openedFile >> chunkSize;

  auto newChunk = new ChunkImpl(chunkPos, chunkSize.val);
  openedFile >> *newChunk;
  openedFile.close();

  return std::unique_ptr<ChunkImpl>(newChunk);
}

bool SaveManager::saveChunk(ChunkImpl const &chunk) {
  using namespace serde;
  std::filesystem::create_directories(chunkSaveDir);
  std::string filePath = chunkSaveDir + "/chunk_" +
                         std::to_string(chunk.chunkPos.x) + "_" +
                         std::to_string(chunk.chunkPos.y) + "_" +
                         std::to_string(chunk.chunkPos.z) + ".chunk";

  zstr::ofstream openedFile(filePath, std::fstream::trunc | std::fstream::binary);
  if (!openedFile) {
    spdlog::warn("Failed to open chunk file: ''", filePath);
    return false;
  }

  Binary<uint8_t> chunkSize(chunk.size().x);
  openedFile << chunkSize << chunk;
  openedFile.close();
  return true;
}
