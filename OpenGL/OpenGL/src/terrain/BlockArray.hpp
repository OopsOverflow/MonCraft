#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Block;

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
