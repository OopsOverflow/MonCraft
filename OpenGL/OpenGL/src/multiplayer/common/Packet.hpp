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

template <typename T, typename = void>
struct is_valid_cont : std::false_type {};

template <typename T>
struct is_valid_cont<T, std::void_t<
  decltype(std::declval<T>().size()),
  decltype(std::declval<T>().begin()),
  decltype(std::declval<T>().end())
>> : std::true_type {};


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

template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
sf::Packet& operator<<(sf::Packet& packet, glm::vec<L, T, Q> const& cont) {
    for (size_t i = 0; i < L; i++) {
        packet << cont[i];
    }
    return packet;
}

template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
sf::Packet& operator>>(sf::Packet& packet, glm::vec<L, T, Q>& vec) {
    for (auto i = 0; i < L; i++) {
        packet >> vec[i];
    }
    return packet;
}
template<typename Container, typename = std::enable_if_t<is_valid_cont<Container>::value>>
sf::Packet& operator<<(sf::Packet& packet, Container const& cont) {
  packet << (sf::Uint64)cont.size();
  for(auto const& val : cont) {
    packet << val;
  }
  return packet;
}

template<typename Container, typename = std::enable_if_t<is_valid_cont<Container>::value>>
sf::Packet& operator>>(sf::Packet& packet, Container& cont) {
  sf::Uint64 size;
  packet >> size;
  cont = Container(size);

  for(sf::Uint64 i = 0; i < size; i++) {
    packet >> cont[i];
  }
  return packet;
}
