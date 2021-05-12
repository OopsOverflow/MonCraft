#pragma once

#include "Block.hpp"

class Debug_Block : public Block {
public:
    static Debug_Block* get() {
      static Debug_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      if(face == BlockFace::TOP)
        return {0, 7};
      else if(face == BlockFace::BOTTOM)
        return {1, 7};
      else if(face == BlockFace::FRONT)
        return {2, 7};
      else if(face == BlockFace::RIGHT)
        return {3, 7};
      else if(face == BlockFace::BACK)
        return {4, 7};
      else
        return {5, 7};
    }

private:
    Debug_Block() : Block(BlockType::Dirt) {}
};
