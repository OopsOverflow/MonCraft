#pragma once

#include "Block.h"

class Oak_Trunk_Block : public Block
{
public:
    static Oak_Trunk_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Oak_Trunk_Block();
};
