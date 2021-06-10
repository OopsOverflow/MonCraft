#pragma once

#include "Block.hpp"

class Birch_Wood_Block : public Block {
public:
    static Birch_Wood_Block* get() {
      static Birch_Wood_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      if(face == BlockFace::TOP || face == BlockFace::BOTTOM)
        return {5, 1};
      return {4, 1};
    }

private:
    Birch_Wood_Block() : Block(BlockType::Birch_Wood) {}
};
