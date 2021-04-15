#ifndef AIR_BLOCK_H
#define AIR_BLOCK_H

#include "Block.h"


class Air_Block : public Block
{
public:
    Air_Block();

    glm::ivec2 getFaceUVs(BlockFace face) const override;

private:
};

#endif // AIR_BLOCK_H
