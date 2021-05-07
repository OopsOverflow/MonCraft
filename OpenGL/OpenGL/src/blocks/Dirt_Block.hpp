#pragma once

#include "Block.hpp"

class Dirt_Block : public Block {
public:
    static Dirt_Block* get() {
      static Dirt_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {1, 0};
    }

private:
    Dirt_Block() : Block(BlockType::Dirt) {}
};
