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
	std::shared_ptr<KeySelector> forward;
	std::shared_ptr<KeySelector> backward;
	std::shared_ptr<KeySelector> left;
	std::shared_ptr<KeySelector> right;
	std::shared_ptr<KeySelector> jump;
	std::shared_ptr<KeySelector> sneak;
	std::shared_ptr<KeySelector> sprint;
	std::shared_ptr<KeySelector> dab;
	std::shared_ptr<KeySelector> changeView;
	//std::unique_ptr<KeySelector> break_;
	//std::unique_ptr<KeySelector> place; mathis is tired
	//std::unique_ptr<KeySelector> select;
	// std::unique_ptr<KeySelector> debug;

};
