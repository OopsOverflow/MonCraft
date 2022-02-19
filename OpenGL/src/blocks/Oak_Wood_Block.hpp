#pragma once

#include "Block.hpp"

class Oak_Wood_Block : public Block {
public:
    static Oak_Wood_Block* get() {
      static Oak_Wood_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      if(face == BlockFace::TOP || face == BlockFace::BOTTOM)
        return {2, 1};
      return {3, 1};
    }

private:
    Oak_Wood_Block() : Block(BlockType::Oak_Wood) {}
};
