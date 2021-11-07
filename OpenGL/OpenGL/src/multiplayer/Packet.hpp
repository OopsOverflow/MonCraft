#pragma once
#include <SFML/Network.hpp>
#include <glm/glm.hpp>

enum class PacketType {
  LOGIN,
  ACK_LOGIN,
  LOGOUT,
  ENTITY_TICK,
  BLOCKS,
  PLAYER_TICK,
  PING,
  CHUNKS,
  ACK_CHUNKS,
  NONE
};


class PacketHeader {

public:
  PacketHeader(PacketType type);
  PacketHeader();

  PacketType getType() const;

private:
  sf::Uint8 type;
};
