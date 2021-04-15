#include "Air_Block.h"

Air_Block::Air_Block() : Block(BlockType::Air)
{ }

//Texture
std::vector<GLfloat> Air_Block::getSideTexCoords() const
{
    return std::vector<GLfloat>();
}
std::vector<GLfloat> Air_Block::getTopTexCoords() const
{
    return std::vector<GLfloat>();
}

std::vector<GLfloat> Air_Block::getBottomTexCoords() const
{
    return std::vector<GLfloat>();
}
