#include "Stone_Block.h"

Stone_Block::Stone_Block()
    : Block(BlockType::Stone)
{}


std::vector<GLfloat> Stone_Block::getSideTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        1.00, 0,
        1.00, 1,
        0.75, 1,
        0.75, 0,
        1.00, 0,
        0.75, 1
    };

    return sideTextures;
}
std::vector<GLfloat> Stone_Block::getTopTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        1.00, 0,
        1.00, 1,
        0.75, 1,
        0.75, 0,
        1.00, 0,
        0.75, 1
    };

    return sideTextures;
}

std::vector<GLfloat> Stone_Block::getBottomTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        1.00, 0,
        1.00, 1,
        0.75, 1,
        0.75, 0,
        1.00, 0,
        0.75, 1
    };

    return sideTextures;
}
