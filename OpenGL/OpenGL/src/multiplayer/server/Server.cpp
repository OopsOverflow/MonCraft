#include "Server.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include "util/Identifier.hpp"
#include "debug/Debug.hpp"
#include "save/SaveManager.hpp"

using namespace glm;

bool Server::stopSignal = false;

Server::Server(unsigned short port)
  : port(port), world(World::getInst())
{
  auto& config = SaveManager::getInst().getConfig();
  renderDistH = config.renderDistH;
  renderDistV = config.renderDistV;

  socket.setBlocking(false);
  if(socket.bind(port) != sf::Socket::Done) {
    throw NetworkError("Failed to bind to port: " + std::to_string(port));
  }
}

bool isVerbose(PacketType type) {
  return
    type != PacketType::ENTITY_TICK &&
    type != PacketType::BLOCKS &&
    type != PacketType::CHUNKS &&
    type != PacketType::ACK_CHUNKS &&
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
      else if(type == PacketType::CHUNKS) handle_chunks(it->second, packet);
      else if(type == PacketType::ACK_CHUNKS) handle_ack_chunks(it->second, packet);
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
    packet << pair.second.player.uid;
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

void Server::packet_ack_login(ClientID const& client, Identifier uid) {
  sf::Packet packet;
  PacketHeader header(PacketType::ACK_LOGIN);
  packet << header << uid;
  socket.send(packet, client.getAddr(), client.getPort());
}

void Server::packet_chunks() {
  static const size_t maxChunks = 5;

  for(auto const& pair : clients) {
    std::vector<std::shared_ptr<Chunk>> chunks;
    for(auto const& cpos : pair.second.waitingChunks) {
      auto chunk = world.chunks.find(cpos);
      if(chunk) {
        chunks.push_back(chunk);
        if(chunks.size() >= maxChunks) break;
      }
    }

    if(chunks.size() != 0) {
      sf::Packet packet;
      PacketHeader header(PacketType::CHUNKS);
      packet << header << (sf::Uint8)chunks.size();
      for(auto const& chunk : chunks)
        packet << (sf::Uint8)chunk->size.x << chunk->chunkPos << *chunk;
      socket.send(packet, pair.first.getAddr(), pair.first.getPort());
    }
  }
}

void Server::sigStop(int signal) {
  stopSignal = true;
}

void Server::run() {
  sf::Clock clock;
  const sf::Time frameDuration = sf::milliseconds(NetworkConfig::SERVER_TICK);

  std::signal(SIGINT, Server::sigStop);

  while(!stopSignal) {
    sf::Time start = clock.getElapsedTime();

    while(poll()) {}
    packet_entity_tick();
    packet_chunks();
    remOldChunks();

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
    packet_ack_login(it->first, it->second.player.uid);
  }
  else {
    Identifier uid = generateIdentifier();
    auto res = clients.emplace(id, Client(uid));

    if(res.second) {
      packet_ack_login(res.first->first, uid);
      beep();
      std::cout << "client connected: " << std::endl;
      std::cout << "uid: " << res.first->second.player.uid << std::endl;
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
    Identifier uid = it->second.player.uid;
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

void Server::handle_chunks(Client& client, sf::Packet& packet) {
  packet >> client.waitingChunks;
  updateWaitingChunks();
}

void Server::handle_ack_chunks(Client& client, sf::Packet& packet) {
  std::vector<ivec3> ack;
  packet >> ack;

  for(auto const& cpos : ack) {
    auto it = std::find(client.waitingChunks.begin(), client.waitingChunks.end(), cpos);
    if(it != client.waitingChunks.end()) client.waitingChunks.erase(it);
  }
}

void Server::handle_blocks(Client const& client, sf::Packet& packet) {
  BlockArray blocks;
  packet >> blocks;

  for(auto const& blockData : blocks) {
    ivec3 cpos = floor(vec3(blockData.pos) / float(world.chunkSize));
    auto chunk = world.chunks.find(cpos);
    if(chunk) {
      ivec3 dpos = blockData.pos - cpos * world.chunkSize;
      chunk->setBlock(dpos, AllBlocks::create_static(blockData.type));
      SaveManager::saveChunk(*chunk);
    }
  }

  packet_blocks(client.player.uid, blocks);
}

void Server::updateWaitingChunks() {
  generator.waitingChunks.clear();
  size_t i = 0, j = 0;
  size_t count = 0;

  for(auto const& pair : clients) {
    count += pair.second.waitingChunks.size();
  }
  count = std::min(count, generator.waitingChunks.capacity());

  while(i < count) {
    for(auto const& pair : clients) {
      auto const& waiting = pair.second.waitingChunks;
      if(j < waiting.size()) {
        generator.waitingChunks.push(waiting.at(j));
        i++;
      }
    }
    j++;
  }
}

void Server::remOldChunks() {
  std::vector<ivec3> playersCpos;
  for(auto const& pair : clients) {
    ivec3 cpos = floor(pair.second.player.getPosition() / float(generator.chunkSize));
    playersCpos.push_back(cpos);
  }

  int delCount = std::max<int>(world.chunks.size() - maxChunks, 0);
  world.chunks.eraseChunks(delCount, [&](ivec3 thisChunkPos) {
    for(auto const& cpos : playersCpos) {
      ivec3 dist = abs(cpos - thisChunkPos);
      bool cond = true;
      cond &= dist.x <= renderDistH + 1;
      cond &= dist.z <= renderDistH + 1;
      cond &= dist.y <= renderDistV + 1;
      if(cond) return false;
    }
    return true;
  });
}
