#include "Overlay.hpp"

#include "gl/ResourceManager.hpp"

#include <iostream>
using namespace ui;

Overlay::Overlay() : selected(0), blockStart(BlockType::Grass)
{
	selector = Box::create();
	selector->setAnchorX(Anchor::CENTER);
	selector->setAnchorY(Anchor::BEGIN);

	for(size_t i = 0; i < nbCells; i += 1) {
		auto slot = BlockSelector::create();
		slot->setAnchorX(Anchor::CENTER);
		slot->setAnchorY(Anchor::CENTER);
		slot->setBlock((BlockType)(i + (int)blockStart));
		cells[i] = slot.get();
		selector->pack_end(move(slot));
	}

	cells[selected]->setSelected(true);

	selector->setOrientation(Box::Orientation::HORIZONTAL);
	add(selector);
}

bool Overlay::select(int selection) {
	static const int nbBlocks = AllBlocks::BlockCount;

	selection -= 1;
	selection = selection % (nbBlocks - 1);
	selection = selection < 0 ? selection + (nbBlocks - 1) : selection;

	int currentSelect = (int)blockStart - 1 + (int)selected;
	currentSelect = currentSelect % (nbBlocks - 1);
	currentSelect = currentSelect < 0 ? currentSelect + (nbBlocks - 1) : currentSelect;

	if(selection == currentSelect) return false;
	cells[selected]->setSelected(false);

	int newSelect;
	int nextScreen;
	int dist1 = nbBlocks - 1 - std::max(currentSelect, selection) + std::min(currentSelect, selection); //not signed
	int dist2 = selection - currentSelect; //signed

	if(dist1 < abs(dist2)) {
		newSelect = dist1;
		if(currentSelect < selection) newSelect *= -1; 
	}
	else {
		newSelect = dist2;
	}
	newSelect += (int)selected;
	nextScreen = newSelect / nbCells;
	newSelect = newSelect % nbCells;
	newSelect = newSelect < 0 ? newSelect + nbCells : newSelect;

	if(nextScreen != 0) {
		int blockID = (int)blockStart - 1 + nextScreen * (int)nbCells; // -1 easier for calculation bc air isn't taken in account
		while(blockID < 0) blockID = nbBlocks - 1 + blockID; 
		blockID %= nbBlocks - 1;
		blockStart = (BlockType)(blockID + 1);

		int airOffset = 0;
		for(size_t i = 0; i < nbCells; i += 1) {
			if(((int)blockStart + i + airOffset) % (nbBlocks) == 0) airOffset += 1;
			BlockType block = (BlockType)(((int)blockStart + i + airOffset) % (nbBlocks));
			cells[i]->setBlock(block);
		}
	}
	selected = newSelect;
	cells[selected]->setSelected(true);

	return true;
}

void Overlay::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Component::draw();
}

std::unique_ptr<Overlay> Overlay::create() {
	auto overlay = std::unique_ptr<Overlay>(new Overlay());
	overlay->initialize();
	return overlay;
}