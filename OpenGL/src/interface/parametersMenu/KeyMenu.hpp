#pragma once

#include <memory>

#include "interface/widgets/ParamList.hpp"
#include "interface/widgets/KeySelector.hpp"

class KeyMenu : public ParamList {

public:
	static std::unique_ptr<KeyMenu> create();
	~KeyMenu();

protected:
	KeyMenu();

private :
	std::unique_ptr<KeySelector> forward;
	std::unique_ptr<KeySelector> backward;
	std::unique_ptr<KeySelector> left;
	std::unique_ptr<KeySelector> right;
	std::unique_ptr<KeySelector> jump;
	std::unique_ptr<KeySelector> sneak;
	std::unique_ptr<KeySelector> sprint;
	std::unique_ptr<KeySelector> dab;
	std::unique_ptr<KeySelector> changeView;
	//std::unique_ptr<KeySelector> break_;
	//std::unique_ptr<KeySelector> place; mathis is tired
	//std::unique_ptr<KeySelector> select;
	// std::unique_ptr<KeySelector> debug;

};
