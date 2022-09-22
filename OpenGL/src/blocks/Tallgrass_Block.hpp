#pragma once

#include "Block.hpp"
#ifdef MONCRAFT_CLIENT
  #include "model/TallgrassModel.hpp"
#endif

class Tallgrass_Block : public Block {
public:
    static Tallgrass_Block* get() {
      static Tallgrass_Block inst;
      return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
      return {7, 6};
    }

    bool isSolid() const override { return false; }
    bool isOpaque() const override { return false; }
    bool isTransparent() const override { return false; }

    #ifdef MONCRAFT_CLIENT
      BlockModel* getModel() const override { return TallgrassModel::get(); }
    #endif
private:
    Tallgrass_Block() : Block(BlockType::Tallgrass) {}
};
