#include "Dirt_Block.h"

Dirt_Block::Dirt_Block()
    : Block(BlockType::Dirt)
{}

//Texture
std::vector<GLfloat> Dirt_Block::getSideTexCoords() const
{
    static const std::vector<GLfloat> sideTextures =
    {
        0.50, 0,
        0.75, 0,
        0.75, 1,
        0.75, 1,
        0.50, 1,
        0.50, 0
    };

    return sideTextures;
}
std::vector<GLfloat> Dirt_Block::getTopTexCoords() const
{
    static const std::vector<GLfloat> topTextures =
    {
        0.50, 0,
        0.75, 0,
        0.75, 1,
        0.75, 1,
        0.50, 1,
        0.50, 0
    };
    return topTextures;
}

std::vector<GLfloat> Dirt_Block::getBottomTexCoords() const
{
    static const std::vector<GLfloat> bottomTextures =
    {
        0.50, 0,
        0.75, 0,
        0.75, 1,
        0.75, 1,
        0.50, 1,
        0.50, 0
    };

    return bottomTextures;
}
