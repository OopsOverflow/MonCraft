#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "blocks/Block.hpp"
#include "multiplayer/common/Packet.hpp"

class BlockArray  {
public:
  void clear();
  bool empty() const;
  void push(glm::ivec3 pos, Block* block);
  void copyToWorld();
  std::vector<glm::ivec3> getChangedChunks() const;
  sf::Packet& serialize(sf::Packet& packet);
  sf::Packet& deserialize(sf::Packet& packet);

private:
  struct elt {
    glm::ivec3 pos;
    std::string ser;
  };
  std::vector<elt> blocks;
};
