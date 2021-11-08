#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "util/Identifier.hpp"
#include "multiplayer/client/Server.hpp"
#include "multiplayer/terrain/PendingChunks.hpp"

class Character;
class World;
namespace sf { class Packet; }

class RealServer: public Server {

public:
  RealServer(std::string url, unsigned short port);
  virtual ~RealServer();

  void ping() override;
  void update() override;
  bool login() override;

  /**
   * Returns nullptr if the player was not created.
   */
  std::shared_ptr<Character> getPlayer() override;

  /**
  * Returns 0 if the player was not created.
  */
  Identifier getUid() override;

private:
  void packet_login();
  void packet_logout();
  void packet_ping();
  void packet_player_tick();
  void packet_blocks();
  void packet_chunks();
  void packet_ack_chunks(std::vector<glm::ivec3> chunks);
  void handle_logout(sf::Packet& packet);
  void handle_blocks(sf::Packet& packet);
  void handle_chunks(sf::Packet& packet);
  void handle_entity_tick(sf::Packet& packet);
  bool poll();


  sf::IpAddress addr;
  unsigned short port;
  sf::UdpSocket socket;
  sf::Time lastUpdate;
  const sf::Time frameDuration;
  sf::Clock clock;
  World& world;
  std::shared_ptr<Character> player;
  Identifier playerUid;
  PendingChunks pendingChunks;

  bool serverAck;
  sf::Time lastServerUpdate;
  const sf::Time timeout = sf::seconds(10);
};
