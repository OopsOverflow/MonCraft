#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "blocks/Block.hpp"
#include "multiplayer/Packet.hpp"

struct BlockArrayElement {
  glm::ivec3 pos;
  std::string ser;
};

class BlockArray : public std::vector<BlockArrayElement> {
public:
  void push(glm::ivec3 pos, Block* block);
  void copyToWorld();
  std::vector<glm::ivec3> getChangedChunks() const;
};
