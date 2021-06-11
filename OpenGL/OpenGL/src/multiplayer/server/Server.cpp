#include "Server.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include "../common/Identifier.hpp"
#include "debug/Debug.hpp"

Server::Server(unsigned short port)
  : port(port)
{
  socket.setBlocking(false);
  if(socket.bind(port) != sf::Socket::Done) {
    throw NetworkError("Failed to bind to port: " + std::to_string(port));
  }
}

bool isVerbose(PacketType type) {
  return type != PacketType::ENTITY_TICK && type != PacketType::PLAYER_TICK;
}

bool Server::poll() {
  sf::Packet packet;
  sf::IpAddress clientAddr;
  unsigned short clientPort;

  auto recv_res = socket.receive(packet, clientAddr, clientPort);

  if (recv_res != sf::Socket::Done) return false;

  PacketHeader header;
  packet >> header;
  auto type = header.getType();
  bool verbose = isVerbose(type);
  if(verbose) std::cout << "Packet " << header << std::endl;

  if(type == PacketType::LOGIN) handle_login(clientAddr, clientPort);
  else if(type == PacketType::LOGOUT) handle_logout(clientAddr, clientPort);

  else {
    ClientID id(clientAddr, clientPort);
    auto it = clients.find(id);
    if(it == clients.end()) {
      std::cout << "[WARN] Client not registered" << std::endl;
    }
    else {

      if(type == PacketType::PING) handle_ping(it->second);
      else if(type == PacketType::PLAYER_TICK) {
        PacketPlayerTick body;
        packet >> body;
        handle_player_tick(it->second, body);
      }

    }
  }

  if(verbose) std::cout << "----------------" << std::endl;
  return true;
}

void Server::broadcast() {
  sf::Packet packet;
  PacketHeader header(PacketType::ENTITY_TICK);

  std::vector<EntityData> entities;

  for(auto const& pair : clients) {
    entities.emplace_back(
      pair.second.player.getPosition(),
      pair.second.player.getIdentifier()
    );
  }

  PacketEntityTick body(entities);
  packet << header << body;

  for(auto const& pair : clients) {
    socket.send(packet, pair.first.getAddr(), pair.first.getPort());
  }
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
    packet_ack_login(it->first, it->second.player.getIdentifier());
  }
  else {
    Identifier uid = generateIdentifier();
    auto res = clients.emplace(id, Client(uid));

    if(res.second) {
      packet_ack_login(res.first->first, uid);
      std::cout << "client connected" << std::endl;
    }
    else {
      std::cout << "[WARN] client insertion failed" << std::endl;
    }
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

void Server::handle_player_tick(Client& client, PacketPlayerTick& body) {
  client.player.setPosition(body.getPlayerPos());
}

void Server::packet_ack_login(ClientID const& client, Identifier uid) {
  sf::Packet packet;
  PacketHeader header(PacketType::ACK_LOGIN);
  PacketAckLogin body(uid);
  packet << header << body;
  socket.send(packet, client.getAddr(), client.getPort());
}
