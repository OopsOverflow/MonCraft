#pragma once

#include "Orientable_Block.hpp"
#ifdef MONCRAFT_CLIENT
    #include "model/StairModel.hpp"
#endif
#include "save/SaveManager.hpp"

class Birch_Stair_Block : public Orientable_Block {
public:
    static Birch_Stair_Block* get() {
        static Birch_Stair_Block inst;
        return &inst;
    }

    bool isOpaque() const override { return false; }

    #ifdef MONCRAFT_CLIENT
        StairModel* getModel() const override { return StairModel::get(); }
    #endif

    glm::ivec2 getFaceUVs(BlockFace face) const override {
        return {6, 1};
    }

    virtual std::ostream& serialize(std::ostream &stream) const override {
        using namespace serde;
        stream << facing;
        return stream;
    }

    virtual Block* deserialize(std::istream &stream) override {
        using namespace serde;
        Facing facing;
        stream >> facing;
        return new Birch_Stair_Block(facing);
    }

private:
    Birch_Stair_Block() : Orientable_Block(BlockType::Birch_Stair) {}
public:
    Birch_Stair_Block(Facing facing) : Orientable_Block(BlockType::Birch_Stair, facing) {}
};
