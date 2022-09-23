#pragma once

#include <SFML/Config.hpp>

enum class PacketType {
  LOGIN,
  ACK_LOGIN,
  LOGOUT,
  ENTITY_TICK,
  BLOCKS,
  PLAYER_TICK,
  PLAYER_ACTION,
  PING,
  CHUNKS,
  ACK_CHUNKS,
  NONE
};

enum class EntityAction {
  BREAK
};

enum class EntityType {
  Character
};


class PacketHeader {

public:
  PacketHeader(PacketType type);
  PacketHeader();

  PacketType getType() const;

private:
  sf::Uint8 type;
};
