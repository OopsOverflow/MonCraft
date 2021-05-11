#pragma once

#include "Block.hpp"

class Sand_Block : public Block {
public:
    static Sand_Block* get() {
      static Sand_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {1, 1};
    }

private:
    Sand_Block() : Block(BlockType::Sand) {}
};
