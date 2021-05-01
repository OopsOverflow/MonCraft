#pragma once

#include "Block.h"

class Sand_Block : public Block
{
public:
    static Sand_Block* get();
    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
    Sand_Block();
};
