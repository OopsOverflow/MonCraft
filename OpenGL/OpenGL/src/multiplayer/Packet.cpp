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
