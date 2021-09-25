#pragma once

#include "Block.hpp"

class Cobalt_Block : public Block {
public:
    static Cobalt_Block* get() {
      static Cobalt_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {2, 3};
    }

private:
    Cobalt_Block() : Block(BlockType::Cobalt) {}
};
