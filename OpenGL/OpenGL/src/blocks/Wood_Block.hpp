#pragma once

#include "Block.hpp"

class Wood_Block : public Block {
public:
    static Wood_Block* get() {
      static Wood_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      if(face == BlockFace::TOP || face == BlockFace::BOTTOM)
        return {2, 1};
      return {3, 1};
    }

private:
    Wood_Block() : Block(BlockType::Wood) {}
};
