#pragma once

#include <memory>

#include "interface/widgets/ParamList.hpp"
#include "interface/widgets/Checkbox.hpp"

class MiscMenu : public ParamList {

public:
	static std::unique_ptr<MiscMenu> create();

protected:
	MiscMenu();

private:
	std::shared_ptr<Checkbox> mobileVersion;
};
