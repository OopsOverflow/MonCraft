#include "Server.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include "debug/Debug.hpp"

Server::Server(unsigned short port)
  : port(port)
{
  if(socket.bind(port) != sf::Socket::Done) {
    throw NetworkError("Failed to bind to port: " + std::to_string(port));
  }
}

bool isVerbose(ClientPacketType type) {
  return type != ClientPacketType::ENTITY_TICK;
}

bool Server::poll() {
  sf::IpAddress clientAddr;
  unsigned short clientPort;

  sf::Packet packet;
  PacketHeader header;

  auto recv_res = socket.receive(packet, clientAddr, clientPort);

  if (recv_res != sf::Socket::Done) return false;

  packet >> header;
  auto type = header.getType();
  bool verbose = isVerbose(type);
  if(verbose) std::cout << "Packet " << header << std::endl;

  if(type == ClientPacketType::LOGIN) handle_login(clientAddr, clientPort);
  else if(type == ClientPacketType::LOGOUT) handle_logout(clientAddr, clientPort);

  else {
    ClientID id(clientAddr, clientPort);
    auto it = clients.find(id);
    if(it == clients.end()) {
      std::cout << "[WARN] Client not registered" << std::endl;
    }
    else {

      if(type == ClientPacketType::PING) handle_ping(it->second);
      else if(type == ClientPacketType::ENTITY_TICK) {
        PacketEntityTick body;
        packet >> body;
        handle_entity_tick(it->second, body);
      }

    }
  }

  if(verbose) std::cout << "-----------------" << std::endl;
  return true;
}

void Server::broadcast() {

}

void Server::run() {
  sf::Clock clock;
  const sf::Time frameDuration = sf::milliseconds(NetworkConfig::SERVER_TICK);

  while(1) {
    sf::Time start = clock.getElapsedTime();

    while(poll()) {}
    broadcast();

    sf::Time elapsed = clock.getElapsedTime() - start;
    if(elapsed < frameDuration) {
      sf::sleep(frameDuration - elapsed);
    }
  }
}

void Server::handle_login(sf::IpAddress clientAddr, unsigned short clientPort) {
  ClientID id(clientAddr, clientPort);

  auto it = clients.find(id);


  if(it != clients.end()) {
    std::cout << "[WARN] Login of already registered client" << std::endl;
  }
  else {
    clients.emplace(id, Client());
    std::cout << "client connected" << std::endl;
  }
}

void Server::handle_logout(sf::IpAddress clientAddr, unsigned short clientPort) {
  ClientID id(clientAddr, clientPort);

  auto it = clients.find(id);

  if(it == clients.end()) {
    std::cout << "[WARN] Logout of unregistered client" << std::endl;
  }
  else {
    clients.erase(it);
    std::cout << "client disconnected" << std::endl;
  }
}

void Server::handle_ping(Client& client) {
  std::cout << "Ping!" << std::endl;
}

void Server::handle_entity_tick(Client& client, PacketEntityTick& body) {
  // client.player.s = body.getPlayerPos();
}
