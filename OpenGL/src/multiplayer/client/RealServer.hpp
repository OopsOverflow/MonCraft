#pragma once

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <rtc/rtc.hpp>

#include "util/Identifier.hpp"
#include "multiplayer/client/Server.hpp"
#include "multiplayer/terrain/PendingChunks.hpp"

class Character;
class World;
namespace sf { class Packet; }

class RealServer: public Server {

public:
  RealServer(std::string host, unsigned short port, bool tls);
  virtual ~RealServer();

  void ping() override;
  void update() override;

  /**
   * Returns nullptr if the player was not created.
   */
  std::shared_ptr<Entity> getPlayer() override;

  /**
  * Returns 0 if the player was not created.
  */
  Identifier getUid() override;
  
  std::string getHost() const { return host; }
  unsigned short getPort() const { return port; }
  bool isWebsocketOpen() const { return socket.isOpen(); }
  bool isDatachannelOpen() const { return channel && channel->isOpen(); }

private:
  void on_message(rtc::message_variant msg);
  bool send(sf::Packet& packet);

  void packet_login();
  void packet_logout();
  void packet_ping();
  void packet_player_tick();
  void packet_blocks();
  void packet_chunks();
  void packet_ack_chunks(std::vector<glm::ivec3> chunks);
  void packet_player_break();
  bool on_packet_recv(sf::Packet& packet);
  void handle_logout(sf::Packet& packet);
  void handle_blocks(sf::Packet& packet);
  void handle_chunks(sf::Packet& packet);
  void handle_entity_tick(sf::Packet& packet);
  void handle_player_action(sf::Packet& packet);


  std::string host;
  unsigned short port;

  rtc::WebSocket socket;
  std::shared_ptr<rtc::PeerConnection> peer;
  std::shared_ptr<rtc::DataChannel> channel;

  sf::Time lastUpdate;
  const sf::Time frameDuration;
  sf::Clock clock;
  World& world;
  std::shared_ptr<Entity> player;
  Identifier playerUid;
  PendingChunks pendingChunks;

  uint32_t worldTime;

  bool serverAck;
  sf::Time lastServerUpdate;
  const sf::Time timeout = sf::seconds(10);
};
