#pragma once

#include <SFML/Network.hpp>
#include <glm/glm.hpp>

enum class ClientPacketType {
  LOGIN,
  LOGOUT,
  ENTITY_TICK,
  PING,
  NONE
};

enum class ServerPacketType {
  ACK_LOGIN,
  ENTITY_TICK,
  PING,
  NONE
};

class PacketHeader {

public:
  PacketHeader(ClientPacketType type);
  PacketHeader();

  ClientPacketType getType() const;

private:
  sf::Uint8 type;

  friend sf::Packet& operator<<(sf::Packet& packet, PacketHeader const& header);
  friend sf::Packet& operator>>(sf::Packet& packet, PacketHeader& header);
  friend std::ostream& operator<<(std::ostream& os, PacketHeader const& header);
};

class PacketEntityTick {
public:
  PacketEntityTick();
  PacketEntityTick(glm::vec3 playerPos);

  glm::vec3 getPlayerPos() const;

private:
  glm::vec3 playerPos;
  friend sf::Packet& operator<<(sf::Packet& packet, PacketEntityTick const& body);
  friend sf::Packet& operator>>(sf::Packet& packet, PacketEntityTick& body);
};

// template<typename Container>
// sf::Packet& operator<<(sf::Packet& packet, Container const& cont) {
//   packet << cont.size();
//   for(auto const& val : cont) {
//     packet << val;
//   }
// }
//
// template<typename Container>
// sf::Packet& operator>>(sf::Packet& packet, Container& cont) {
//   sf::Uint64 size;
//   packet >> size;
//   cont = Container(size);
//
//   for(auto i = 0; i < size; i++) {
//     packet >> cont[i];
//   }
// }
//
// template<typename StaticContainer, size_t N>
// sf::Packet& operator<<(sf::Packet& packet, StaticContainer const& cont) {
//   for(size_t i = 0; i < N; i++) {
//     packet << cont[i];
//   }
// }
//
// template<typename StaticContainer, size_t N>
// sf::Packet& operator>>(sf::Packet& packet, StaticContainer& vec) {
//   for(auto i = 0; i < N; i++) {
//     packet >> vec[i];
//   }
// }

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
