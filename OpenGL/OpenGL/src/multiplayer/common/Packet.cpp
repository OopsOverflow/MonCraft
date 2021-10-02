#include "Packet.hpp"

// #define VERBOSE_PACKET

#ifdef VERBOSE_PACKET
#include <sstream>
#endif

PacketHeader::PacketHeader(PacketType type)
  : type((sf::Uint8)type)
{ }

PacketHeader::PacketHeader()
  : PacketHeader(PacketType::NONE)
{ }


PacketType PacketHeader::getType() const {
  return (PacketType)type;
}

sf::Packet& operator<<(sf::Packet& packet, PacketHeader const& header) {
    #ifdef VERBOSE_PACKET
      std::stringstream ss;
      ss << header;
      packet << ss.str();
    #endif
    return packet << header.type;
}

sf::Packet& operator>>(sf::Packet& packet, PacketHeader& header) {
    #ifdef VERBOSE_PACKET
      std::string s;
      packet >> s;
    #endif
    return packet >> header.type;
}

std::ostream& operator<<(std::ostream& os, PacketHeader const& header) {
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
