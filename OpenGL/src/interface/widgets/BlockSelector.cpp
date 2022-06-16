#include "BlockSelector.hpp"

using namespace ui;

BlockSelector::BlockSelector()
  : selected(false), pos(212, 1178), block(BlockType::Dirt)
{
    img = Image::create(pos, {20, 20});
    img->setSize({60, 60});
    setBlock(block);
    onClick([&] { setSelected(!getSelected()); });
    add(img);
}

bool BlockSelector::getSelected() const {
    return selected;
}

void BlockSelector::setSelected(bool selected) {
    this->selected = selected;
    glm::ivec2 off = img->getTextureOffset();
    if(selected) off = pos + glm::vec2(23, 0);
    else off = pos;

    img->setTextureOffset(off);
}

std::unique_ptr<BlockSelector> BlockSelector::create() {
    auto check = std::unique_ptr<BlockSelector>(new BlockSelector());
    check->initialize();
    return check;
}

void BlockSelector::setBlock(BlockType block) {
    if(!img || block == BlockType::Air)return;
    while(img->getChildren().size() > 0) img->remove(img->getChildren().at(0).get());

    glm::ivec2 position;
    position.x = 51 + (((int)block - 1) % 8) * 16;
    position.y = 2160 - ( 847 + (((int)block - 1) / 8) * 16);
    
    auto blockTex = Image::create(position, {16, 16});
    blockTex->setSize({48, 48});
    blockTex->setAnchorX(Anchor::CENTER);
    blockTex->setAnchorY(Anchor::CENTER);
    img->add(move(blockTex));

    this->block = block;
}

BlockType BlockSelector::getBlock() const {
    return block;
}