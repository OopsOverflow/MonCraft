#pragma once

#include <string>
#include <map>
#include <SFML/Network.hpp>
#include "../common/Packet.hpp"

#include "Client.hpp"

class Server {

public:
  Server(unsigned short port);

  void run();

private:
  void broadcast(sf::Packet& packet);
  bool poll();
  void handle_login(sf::IpAddress clientAddr, unsigned short clientPort);
  void handle_logout(sf::IpAddress clientAddr, unsigned short clientPort);
  void handle_ping(Client& client);
  void handle_player_tick(Client& client, sf::Packet& packet);
  void packet_entity_tick();
  void packet_logout(Identifier uid);
  void packet_ack_login(ClientID const& client, Identifier uid);
  void beep();

  unsigned short port;
  sf::UdpSocket socket;
  std::map<ClientID, Client> clients;
};
