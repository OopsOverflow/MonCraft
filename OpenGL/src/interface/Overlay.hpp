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
	static std::unique_ptr<Overlay> create();
	void draw() override;
	
	bool select(int selection);
	size_t getSelected() const { return selected; }
	BlockType getCurrentBlock() { return cells[selected]->getBlock(); }

	std::shared_ptr<ui::Box> selector;
private:
	Overlay();
	size_t selected;
	BlockType blockStart;
	BlockSelector* cells[nbCells];
};
