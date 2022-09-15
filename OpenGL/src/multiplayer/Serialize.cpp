#include "Serialize.hpp"

#include <sstream>
#include <string>
#include <vector>

#include "entity/Node.hpp"
#include "save/SaveManager.hpp"
#include "terrain/BlockArray.hpp"

class Chunk;

sf::Packet& serde::operator<<(sf::Packet& packet, Entity const& entity) {
	packet << entity.bodyNode.loc;
	packet << entity.bodyNode.rot;
	packet << entity.headNode.rot;
	packet << entity.speed;
	packet << entity.accel;
	packet << entity.direction;
  packet << entity.dab;
  packet << entity.god;
	packet << (sf::Uint8)entity.state;
	return packet;
}

sf::Packet& serde::operator>>(sf::Packet& packet, Entity& entity) {
	sf::Uint8 state;
	packet >> entity.bodyNode.loc;
	packet >> entity.bodyNode.rot;
	packet >> entity.headNode.rot;
	packet >> entity.speed;
	packet >> entity.accel;
	packet >> entity.direction;
  packet >> entity.dab;
  packet >> entity.god;
	packet >> state;
	entity.state = (State)state;
	return packet;
}

sf::Packet& serde::consume(Entity& e, sf::Packet& packet) {
	decltype(e.bodyNode.loc) loc;
	decltype(e.bodyNode.rot) rot;
	decltype(e.headNode.rot) headRot;
	decltype(e.speed) speed;
	decltype(e.accel) accel;
	decltype(e.direction) direction;
  decltype(e.dab) dab;
  decltype(e.god) god;
	sf::Uint8 state;
	packet >> loc;
	packet >> rot;
	packet >> headRot;
	packet >> speed;
	packet >> accel;
	packet >> direction;
  packet >> dab;
  packet >> god;
	packet >> state;
	return packet;
}

sf::Packet& serde::operator<<(sf::Packet& packet, Action const& action) {
	packet << (sf::Uint8)action;
	return packet;
}

sf::Packet& serde::operator>>(sf::Packet& packet, Action& a) {
	sf::Uint8 action;
	packet >> action;
	a = (Action)action;
	return packet;
}

sf::Packet& serde::consume(Action& a, sf::Packet& packet) {
	sf::Uint8 action;
	packet >> action;
	return packet;
}

sf::Packet& serde::operator<<(sf::Packet& packet, PacketHeader const& header) {
  #ifdef VERBOSE_PACKET
    std::stringstream ss;
    ss << header;
    packet << ss.str();
  #endif
  return packet << (sf::Uint8)header.getType();
}

sf::Packet& serde::operator>>(sf::Packet& packet, PacketHeader& header) {
  #ifdef VERBOSE_PACKET
    std::string s;
    packet >> s;
  #endif
	sf::Uint8 type;
	packet >> type;
	header = PacketHeader((PacketType)type);
  return packet;
}

std::ostream& serde::operator<<(std::ostream& os, PacketHeader const& header) {
  PacketType type = header.getType();
  if(type == PacketType::LOGIN) os << "LOGIN";
  else if(type == PacketType::ACK_LOGIN) os << "ACK_LOGIN";
  else if(type == PacketType::LOGOUT) os << "LOGOUT";
  else if(type == PacketType::PLAYER_TICK) os << "PLAYER_TICK";
  else if(type == PacketType::ENTITY_TICK) os << "ENTITY_TICK";
  else if(type == PacketType::BLOCKS) os << "BLOCKS";
  else if(type == PacketType::PING) os << "PING";
  else if(type == PacketType::CHUNKS) os << "CHUNKS";
  else if(type == PacketType::ACK_CHUNKS) os << "ACK_CHUNKS";
  else if(type == PacketType::NONE) os << "NONE";
  else os << "<unknown: " << (int)type << ">";
  return os;
}

sf::Packet& serde::operator<<(sf::Packet& packet, BlockArray const& record) {
  using namespace serde;
  packet << (sf::Uint64)record.size();

  for (auto const& elt : record) {
      packet << elt.pos << elt.ser;
  }

  return packet;
}

sf::Packet& serde::operator>>(sf::Packet& packet, BlockArray& record) {
  using namespace serde;
  sf::Uint64 count;
  packet >> count;

  for (sf::Uint64 i = 0; i < count; i++) {
    BlockArrayElement el;
    packet >> el.pos >> el.ser;
    record.push_back(el);
  }

  return packet;
}

sf::Packet& serde::operator<<(sf::Packet &packet, ChunkImpl const& chunk) {
  std::ostringstream stream;
  stream << chunk;
  packet << stream.str();
  return packet;
}

sf::Packet& serde::operator>>(sf::Packet &packet, ChunkImpl &chunk) {
  std::string str;
  packet >> str;
  std::stringstream stream(str);
  stream >> chunk;
  return packet;
}
