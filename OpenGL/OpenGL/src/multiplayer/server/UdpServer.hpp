#pragma once

#include <SFML/Network/UdpSocket.hpp>

#include "multiplayer/server/Server.hpp"

class ClientID;
namespace sf { class Packet; }


class UdpServer : public Server {

public:
  UdpServer(unsigned short port);
  virtual ~UdpServer();

  void run() override;

protected:
  void send(sf::Packet& packet, ClientID client) override;

private:
  bool poll();

  sf::UdpSocket socket;

  static void sigStop(int signal);
  static bool stopSignal;
};
