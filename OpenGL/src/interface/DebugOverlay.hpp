#pragma once

#include <memory>

#include "ui/Box.hpp"
#include "ui/Text.hpp"

class Server;
class World;

class DebugOverlay : public ui::Box
{
public:
	static std::unique_ptr<DebugOverlay> create(std::shared_ptr<Server> server);
	void draw() override;

private:
	DebugOverlay(std::shared_ptr<Server> server);
	std::shared_ptr<ui::Text> text_fps;
	std::shared_ptr<ui::Text> text_posPlayer;
	std::shared_ptr<ui::Text> text_players;
	std::shared_ptr<ui::Text> text_uid;
	std::shared_ptr<ui::Text> text_gameTime;

	std::shared_ptr<Server> server;
	World& world;
};
