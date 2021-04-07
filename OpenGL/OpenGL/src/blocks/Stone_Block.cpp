#include "Stone_Block.h"

Stone_Block::Stone_Block()
    : Block(Block_Type::Stone)
{}


std::vector<GLfloat> Stone_Block::getSideTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}
std::vector<GLfloat> Stone_Block::getTopTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}

std::vector<GLfloat> Stone_Block::getBottomTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}
