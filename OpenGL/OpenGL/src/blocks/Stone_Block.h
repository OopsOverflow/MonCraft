#ifndef STONE_BLOCK_H
#define STONE_BLOCK_H

#include "Block.h"

class Stone_Block : public Block
{
public:
    Stone_Block();

    std::vector<GLfloat> getSideTexCoords() const override;
    std::vector<GLfloat> getTopTexCoords() const override;
    std::vector<GLfloat> getBottomTexCoords() const override;

};

#endif // STONE_BLOCK_H
