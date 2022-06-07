#pragma once

#include <memory>

#include "ui/Button.hpp"
#include "ui/Component.hpp"
#include "ui/Box.hpp"
#include "widgets/BlockSelector.hpp"

static const size_t nbCells = 8;
class Overlay : public ui::Component
{
public:
	Overlay();
	void draw() override;
	
	bool select(int selection);
	size_t getSelected() const { return selected; }
	BlockType getCurrentBlock() { return cells[selected]->getBlock(); }

	std::unique_ptr<ui::Box> selector;
private:
	size_t selected;
	BlockType blockStart;
	BlockSelector* cells[nbCells];
};
