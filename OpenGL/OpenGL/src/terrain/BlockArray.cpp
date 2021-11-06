#include "BlockArray.hpp"
#include "blocks/AllBlocks.hpp"
#include "terrain/World.hpp"
#include <sstream>
#include <set>
#include "debug/Debug.hpp"

bool BlockArray::empty() const {
  return blocks.empty();
}

void BlockArray::clear() {
  blocks.clear();
}

std::vector<glm::ivec3> BlockArray::getChangedChunks() const {
  float chunkSize = World::getInst().chunkSize; // TODO: in config
  std::vector<glm::ivec3> res;
  for(auto const& elt : blocks) {
    glm::ivec3 cpos = floor(glm::vec3(elt.pos) / chunkSize);
    if(std::find(res.begin(), res.end(), cpos) == res.end())
      res.push_back(cpos);
  }
  return res;
}

void BlockArray::push(glm::ivec3 pos, Block* block) {
  std::ostringstream ss;
  AllBlocks::serialize(ss, block);
  blocks.push_back({ pos, ss.str() });
}

void BlockArray::copyToWorld() {
  World& world = World::getInst();

  for(auto const& elt : blocks) {
    std::istringstream ss(elt.ser);
    world.setBlock(elt.pos, AllBlocks::deserialize(ss));
    Block* block = world.getBlock(elt.pos);
    std::cout << (int)block->type << std::endl;
  }
}

sf::Packet& BlockArray::serialize(sf::Packet& packet) {
  packet << (sf::Uint64)blocks.size();

  for (auto const& elt : blocks) {
      packet << elt.pos << elt.ser;
  }

  return packet;
}

sf::Packet& BlockArray::deserialize(sf::Packet& packet) {
  sf::Uint64 count;
  packet >> count;

  for (sf::Uint64 i = 0; i < count; i++) {
    elt el;
    packet >> el.pos >> el.ser;
    blocks.push_back(el);
  }

  return packet;
}
