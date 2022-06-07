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
	add(move(selector));
}

bool Overlay::select(int selection) {
	if(selection == selected) return false;

	cells[selected]->setSelected(false);
	int nextScreen = 0;
	while(selection >= (int)nbCells) {
		selection -= nbCells;
		nextScreen += 1;
	}
	while(selection < 0) {
		selection += nbCells;
		nextScreen -= 1;
	}
	
	if(nextScreen != 0) {
		static const int nbBlocks = (int)BlockType::Birch_Stair + 1;//Last block of the array here
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
	selected = selection;
	cells[selected]->setSelected(true);

	return true;
}

void Overlay::draw() {
	if (parent) {
		setSize(parent->getSize());
	}
	Component::draw();
}
