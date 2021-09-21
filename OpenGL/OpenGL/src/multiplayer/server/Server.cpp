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
  return
    type != PacketType::ENTITY_TICK &&
    type != PacketType::BLOCKS &&
    type != PacketType::PLAYER_TICK;
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
      else if(type == PacketType::BLOCKS) handle_blocks(it->second, packet);
      else if(type == PacketType::PLAYER_TICK) handle_player_tick(it->second, packet);

    }
  }

  if(verbose) std::cout << "----------------" << std::endl;
  return true;
}

void Server::packet_entity_tick() {
  sf::Packet packet;
  PacketHeader header(PacketType::ENTITY_TICK);
  packet << header;
  packet << (sf::Uint64)clients.size();

  for(auto const& pair : clients) {
    packet << pair.second.player.getIdentifier();
    packet << pair.second.player;
  }
  broadcast(packet);
}

void Server::beep() {
  std::cout << '\a';
  std::cout.flush();
}

void Server::broadcast(sf::Packet& packet) {
  for(auto const& pair : clients) {
    socket.send(packet, pair.first.getAddr(), pair.first.getPort());
  }
}

void Server::packet_logout(Identifier uid) {
  sf::Packet packet;
  PacketHeader header(PacketType::LOGOUT);
  packet << header << uid;
  broadcast(packet);
}

void Server::packet_blocks(Identifier uid, BlockArray changedBlocks) {
  sf::Packet packet;
  PacketHeader header(PacketType::BLOCKS);
  packet << header << uid << changedBlocks;
  broadcast(packet);
}

void Server::run() {
  sf::Clock clock;
  const sf::Time frameDuration = sf::milliseconds(NetworkConfig::SERVER_TICK);

  while(1) {
    sf::Time start = clock.getElapsedTime();

    while(poll()) {}
    packet_entity_tick();

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
      beep();
      std::cout << "client connected: " << std::endl;
      std::cout << "uid: " << res.first->second.player.getIdentifier() << std::endl;
      std::cout << "addr: " << res.first->first.getAddr() << std::endl;
      std::cout << "port: " << res.first->first.getPort() << std::endl;
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
    Identifier uid = it->second.player.getIdentifier();
    clients.erase(it);
    packet_logout(uid);
    beep();
    std::cout << "client disconnected" << std::endl;
  }
}

void Server::handle_ping(Client& client) {
  std::cout << "Ping!" << std::endl;
  beep();
}

void Server::handle_player_tick(Client& client, sf::Packet& packet) {
  packet >> client.player;
}

void Server::packet_ack_login(ClientID const& client, Identifier uid) {
  sf::Packet packet;
  PacketHeader header(PacketType::ACK_LOGIN);
  packet << header << uid;
  socket.send(packet, client.getAddr(), client.getPort());
}

void Server::handle_blocks(Client const& client, sf::Packet& packet) {
  BlockArray blocks;
  packet >> blocks;
  packet_blocks(client.player.getIdentifier(), blocks);
}
