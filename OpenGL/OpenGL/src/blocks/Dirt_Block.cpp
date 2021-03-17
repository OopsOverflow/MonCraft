#include "Dirt_Block.h"

Dirt_Block::Dirt_Block()
    : Block(Block_Type::Dirt)
{}

//Texture
std::vector<GLfloat> Dirt_Block::getSideTextureCoords()
{
    static const std::vector<GLfloat> sideTextures =
    {
        // TODO
    };

    return sideTextures;
}
std::vector<GLfloat> Dirt_Block::getTopTextureCoords()
{
    static const std::vector<GLfloat> topTextures =
    {
        // TODO
    };
    return topTextures;
}

std::vector<GLfloat> Dirt_Block::getBottomTextureCoords()
{
    static const std::vector<GLfloat> bottomTextures =
    {
        // TODO
    };

    return bottomTextures;
}