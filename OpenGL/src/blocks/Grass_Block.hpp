#pragma once

#include "Block.hpp"

class Grass_Block : public Block {
public:
    static Grass_Block* get() {
      static Grass_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      if(face == BlockFace::TOP)
        return {0, 0};
      else if(face == BlockFace::BOTTOM)
        return {1, 0};
      return {2, 0};
    }

private:
    Grass_Block() : Block(BlockType::Grass) {}
};
