#pragma once

#include "Block.hpp"
#include "geometry/WaterGeometry.hpp"

class Water_Block : public Block {
public:
    static Water_Block* get() {
        static Water_Block inst;
        return &inst;
    }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
        return { 3, 2 };
    }

    bool isSolid() const override { return false; }
    bool isTransparent() const override { return true; }
    bool isWater() const override { return true; }
    BlockGeometry* getGeometry() const override { return WaterGeometry::get(); }

private:
    Water_Block() : Block(BlockType::Water) {}
};
