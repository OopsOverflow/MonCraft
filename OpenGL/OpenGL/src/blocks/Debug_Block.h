#ifndef DEBUG_BLOCK_H
#define DEBUG_BLOCK_H

#include "Block.h"

class Debug_Block : public Block
{
public:
    Debug_Block();

    glm::ivec2 getFaceUVs(BlockFace face) const override;

};

#endif // DEBUG_BLOCK_H
