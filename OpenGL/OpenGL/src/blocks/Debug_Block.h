#pragma once

#include "Block.h"

class Debug_Block : public Block
{
public:
    static Debug_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Debug_Block();
};
