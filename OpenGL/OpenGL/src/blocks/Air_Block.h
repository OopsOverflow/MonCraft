#ifndef AIR_BLOCK_H
#define AIR_BLOCK_H

#include "Block.h"


class Air_Block : public Block
{
public:
    Air_Block();

    std::vector<GLfloat> getSideTexCoords() const override;
    std::vector<GLfloat> getTopTexCoords() const override;
    std::vector<GLfloat> getBottomTexCoords() const override;

private:
};

#endif // AIR_BLOCK_H
