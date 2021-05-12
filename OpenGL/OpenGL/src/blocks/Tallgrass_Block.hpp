#pragma once

#include "Block.hpp"
#include "geometry/TallgrassGeometry.hpp"

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
    bool isTransparent() const override { return true; }
    BlockGeometry* getGeometry() const override { return TallgrassGeometry::get(); }

private:
    Tallgrass_Block() : Block(BlockType::Tallgrass) {}
};
