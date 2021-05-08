#pragma once

#include "Block.hpp"

class Stone_Block : public Block {
public:
    static Stone_Block* get() {
      static Stone_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {0, 1};
    }

private:
    Stone_Block() : Block(BlockType::Stone) {}
};
