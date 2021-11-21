#pragma once

#include <memory>

#include "interface/widgets/ParamList.hpp"

class MiscMenu : public ParamList {

public:
	static std::unique_ptr<MiscMenu> create();

protected:
	MiscMenu();

private:
	// std::unique_ptr<ui::Box> mobileVersion;
};
