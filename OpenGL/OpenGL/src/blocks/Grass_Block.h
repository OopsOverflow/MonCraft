#ifndef GRASS_BLOCK_H
#define GRASS_BLOCK_H

#include "Block.h"

class Grass_Block : public Block
{
public:
    Grass_Block();

    std::vector<GLfloat> getSideTexCoords() const override;
    std::vector<GLfloat> getTopTexCoords() const override;
    std::vector<GLfloat> getBottomTexCoords() const override;

};

#endif // GRASS_BLOCK_H
