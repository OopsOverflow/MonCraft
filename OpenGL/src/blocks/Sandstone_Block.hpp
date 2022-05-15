#pragma once

#include "Block.hpp"

class Sandstone_Block : public Block {
public:
    static Sandstone_Block* get() {
      static Sandstone_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {2, 2};
    }

private:
    Sandstone_Block() : Block(BlockType::Sandstone) {}
};
