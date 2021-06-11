#pragma once
#include <SFML/Network.hpp>
#include <glm/glm.hpp>

enum class PacketType {
  LOGIN,
  ACK_LOGIN,
  LOGOUT,
  ENTITY_TICK,
  PLAYER_TICK,
  PING,
  NONE
};

class PacketHeader {

public:
  PacketHeader(PacketType type);
  PacketHeader();

  PacketType getType() const;

private:
  sf::Uint8 type;

  friend sf::Packet& operator<<(sf::Packet& packet, PacketHeader const& header);
  friend sf::Packet& operator>>(sf::Packet& packet, PacketHeader& header);
  friend std::ostream& operator<<(std::ostream& os, PacketHeader const& header);
};

template<typename T>
sf::Packet& operator<<(sf::Packet& packet, std::vector<T> const& cont) {
  packet << (sf::Uint64)cont.size();
  for(auto const& val : cont) {
    packet << val;
  }
  return packet;
}

template<typename T>
sf::Packet& operator>>(sf::Packet& packet, std::vector<T>& cont) {
  sf::Uint64 size;
  packet >> size;
  cont = std::vector<T>(size);

  for(sf::Uint64 i = 0; i < size; i++) {
    packet >> cont[i];
  }
  return packet;
}

template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
sf::Packet& operator<<(sf::Packet& packet, glm::vec<L, T, Q> const& cont) {
  for(size_t i = 0; i < L; i++) {
    packet << cont[i];
  }
  return packet;
}

template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
sf::Packet& operator>>(sf::Packet& packet, glm::vec<L, T, Q>& vec) {
  for(auto i = 0; i < L; i++) {
    packet >> vec[i];
  }
  return packet;
}
