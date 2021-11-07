#pragma once

#include "Orientable_Block.hpp"
#include "model/StairModel.hpp"
#include "save/SaveManager.hpp"

class Oak_Stair_Block : public Orientable_Block {
public:
    static Oak_Stair_Block* get() {
        static Oak_Stair_Block inst;
        return &inst;
    }

    bool isOpaque() const override { return false; }

    StairModel* getModel() const override { return StairModel::get(); }

    glm::ivec2 getFaceUVs(BlockFace face) const override {
        return {7, 1};
    }

    virtual std::ostream& serialize(std::ostream &stream) const override {
        using namespace Serde;
        stream << facing;
        return stream;
    }

    virtual Block* deserialize(std::istream &stream) override {
        using namespace Serde;
        Facing facing;
        stream >> facing;
        return new Oak_Stair_Block(facing);
    }

private:
    Oak_Stair_Block() : Orientable_Block(BlockType::Oak_Stair) {}
public:
    Oak_Stair_Block(Facing facing) : Orientable_Block(BlockType::Oak_Stair, facing) {}
};
