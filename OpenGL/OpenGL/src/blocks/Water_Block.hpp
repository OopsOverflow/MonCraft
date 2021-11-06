#pragma once

#include "Block.hpp"
#include "model/WaterModel.hpp"

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
    bool isOpaque() const override { return false; }
    bool isTransparent() const override { return true; }
    bool isLiquid() const override { return true; }
    BlockModel* getModel() const override { return WaterModel::get(); }

private:
    Water_Block() : Block(BlockType::Water) {}
};
