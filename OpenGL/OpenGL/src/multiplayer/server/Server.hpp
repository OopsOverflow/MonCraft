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
  bool poll();
  void broadcast();
  void handle_login(sf::IpAddress clientAddr, unsigned short clientPort);
  void handle_logout(sf::IpAddress clientAddr, unsigned short clientPort);
  void handle_ping(Client& client);
  void handle_player_tick(Client& client, PacketPlayerTick& body);
  void packet_ack_login(ClientID const& client, Identifier uid);

  unsigned short port;
  sf::UdpSocket socket;
  std::map<ClientID, Client> clients;
};
