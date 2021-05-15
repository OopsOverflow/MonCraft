#pragma once

#include "Block.hpp"

class Ice_Block : public Block {
public:
    static Ice_Block* get() {
      static Ice_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {0, 2};
    }

private:
    Ice_Block() : Block(BlockType::Ice) {}
};
