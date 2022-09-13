#include "DebugOverlay.hpp"

#include <glm/glm.hpp>
#include <iomanip>
#include <sstream>

#include "debug/Debug.hpp"
#include "entity/Entities.hpp"
#include "entity/character/Character.hpp"
#include "gl/ResourceManager.hpp"
#include "multiplayer/client/Server.hpp"
#include "multiplayer/client/RealServer.hpp"
#include "terrain/World.hpp"
#include "ui/Component.hpp"
#include "ui/style/Type.hpp"

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
	text_connectivity = Text::create("Connectivity : ", font);

	pack_start(text_fps);
	pack_start(text_posPlayer);
	pack_start(text_players);
	pack_start(text_uid);
	pack_start(text_gameTime);
	pack_start(text_connectivity);
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

	text_connectivity->setColor({ 1.0f, 1.0f, 1.0f, 1.f });
	text_connectivity->setAnchorY(Anchor::END);
	text_connectivity->setFontSize(.35f);
}

void DebugOverlay::draw() {
	setSize(glm::ivec2(parent->getSize().x, 0));

	std::ostringstream text;
	text << "FPS : " << (int)(1000 / (float)world.dt);
	text_fps->setText(text.str());

	text.str(""); // "clears" the string stream
	text << "Pos : " << std::fixed << std::setprecision(3) << server->getPlayer()->getPosition();
	text_posPlayer->setText(text.str());

	text.str(""); // "clears" the string stream
	text << "Players online : " << world.entities.count();
	text_players->setText(text.str());

	text.str(""); // "clears" the string stream
	text << "UID : " << server->getUid();
	text_uid->setText(text.str());

	text.str(""); // "clears" the string stream
	int hour = (int)(world.t * convertFactor * 0.001f);
	int min = (int)(((world.t * convertFactor * 0.001f) - hour) * 60.f);
	text << "Game Time : ";
	if(hour < 10) text << 0;
	text << hour << ":";
	if(min < 10) text << 0;
	text << min;
	text_gameTime->setText(text.str());
	
	text.str("");
	text << "Connectivity: ";
	const ServerState state = server->getState();
	if (state == ServerState::CONNECTED) text << "Connected";
	else if (state == ServerState::CONNECTING) text << "Connecting";
	else if (state == ServerState::DISCONNECTED) text << "Disconnected";
	else text << "Unknown";
	text << ", ";
	if(server->getUid() != 0) {
		auto realServer = std::static_pointer_cast<RealServer>(server);
		text << "Multiplayer:"
				 << " Host="       << realServer->getHost()
				 << " Port="      << realServer->getPort()
				 << " WebSocket=" << realServer->isWebsocketOpen()
			   << " WebRTC="    << realServer->isDatachannelOpen();
	}
	else {
		text << "Singleplayer";
	}
	text_connectivity->setText(text.str());


	Box::draw();
}


std::unique_ptr<DebugOverlay> DebugOverlay::create(std::shared_ptr<Server> server) {
	auto debug = std::unique_ptr<DebugOverlay>(new DebugOverlay(std::move(server)));
	debug->initialize();
	return debug;
}