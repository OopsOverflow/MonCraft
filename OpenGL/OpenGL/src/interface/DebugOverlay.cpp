#include "DebugOverlay.hpp"
#include "gl/ResourceManager.hpp"

using namespace ui;

DebugOverlay::DebugOverlay(std::shared_ptr<Server> server) :
	server(server),
	world(World::getInst())
{
	auto font = ResourceManager::getFont("roboto");
	text_fps = Text::create("Fps : ", font);;
	text_posPlayer = Text::create("Pos : ", font);
	text_players = Text::create("Players : ", font);
	text_uid = Text::create("UID : ", font);
	text_gameTime = Text::create("Time : ", font);

	pack_end(text_fps.get());
	pack_end(text_posPlayer.get());
	pack_end(text_players.get());
	pack_end(text_uid.get());
	pack_end(text_gameTime.get());
	setPadding(glm::ivec2(10));
	setGap(10);

	text_fps->setColor({ 1.0f, 1.0f, 1.0f, 1.f });
	text_fps->setAnchorY(Anchor::END);
	text_fps->setFontSize(.35f);

	text_posPlayer->setColor({ 1.0f, 1.0f, 1.0f, 1.f });
	text_posPlayer->setAnchorY(Anchor::END);
	text_posPlayer->setFontSize(.35f);

	text_players->setColor({ 1.0f, 1.0f, 1.0f, 1.f });
	text_players->setAnchorY(Anchor::END);
	text_players->setFontSize(.35f);

	text_uid->setColor({ 1.0f, 1.0f, 1.0f, 1.f });
	text_uid->setAnchorY(Anchor::END);
	text_uid->setFontSize(.35f);

	text_gameTime->setColor({ 1.0f, 1.0f, 1.0f, 1.f });
	text_gameTime->setAnchorY(Anchor::END);
	text_gameTime->setFontSize(.35f);

}

void DebugOverlay::draw() {
	setSize(glm::ivec2(parent->getSize().x, 0));

	std::ostringstream text;
	text << "FPS : " << (int)(1.f / world.dt);
	text_fps->setText(text.str());

	text.str(""); // "clears" the string stream
	text << "Pos : " << std::fixed << std::setprecision(3) << server->getPlayer()->getPosition();
	text_posPlayer->setText(text.str());

	text.str(""); // "clears" the string stream
	text << "Players online : " << world.entities.count();
	text_players->setText(text.str());

	text.str(""); // "clears" the string stream
	text << "UID : " << server->getPlayer()->uid;
	text_uid->setText(text.str());

	text.str(""); // "clears" the string stream
	text << "Game Time : " << std::fixed << std::setprecision(3) << world.t;
	text_gameTime->setText(text.str());


	Box::draw();
}
