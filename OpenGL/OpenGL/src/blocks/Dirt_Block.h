#ifndef DIRT_BLOCK_H
#define DIRT_BLOCK_H

#include "Block.h"


class Dirt_Block : public Block
{
public:
    Dirt_Block();

    std::vector<GLfloat> getSideTexCoords() const override;
    std::vector<GLfloat> getTopTexCoords() const override;
    std::vector<GLfloat> getBottomTexCoords() const override;

private:
};

#endif // DIRT_BLOCK_H
