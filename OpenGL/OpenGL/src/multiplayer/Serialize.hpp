#pragma once
#include "entity/Entity.hpp"
#include "terrain/BlockArray.hpp"
#include "terrain/AbstractChunk.hpp"
#include "util/Serde.hpp"

#include <SFML/Network.hpp>

namespace Serde {
  sf::Packet& consume(Entity& e, sf::Packet& packet);
  sf::Packet& operator<<(sf::Packet& packet, Entity const& entity);
  sf::Packet& operator>>(sf::Packet& packet, Entity& entity);

  sf::Packet& operator<<(sf::Packet& packet, PacketHeader const& header);
  sf::Packet& operator>>(sf::Packet& packet, PacketHeader& header);
  std::ostream& operator<<(std::ostream& os, PacketHeader const& header);

  sf::Packet& operator<<(sf::Packet& packet, BlockArray const& record);
  sf::Packet& operator>>(sf::Packet& packet, BlockArray& record);

  sf::Packet& operator<<(sf::Packet &packet, AbstractChunk const& chunk);
  sf::Packet& operator>>(sf::Packet &packet, AbstractChunk& chunk);

  template <typename T, typename = void>
  struct is_valid_cont : std::false_type {};

  template <typename T>
  struct is_valid_cont<T, std::void_t<
    decltype(std::declval<T>().size()),
    decltype(std::declval<T>().begin()),
    decltype(std::declval<T>().end())
  >> : std::true_type {};

  template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
  sf::Packet& operator<<(sf::Packet& packet, glm::vec<L, T, Q> const& cont) {
      for (glm::length_t i = 0; i < L; i++) {
          packet << cont[i];
      }
      return packet;
  }

  template<glm::length_t L, typename T, glm::qualifier Q = glm::defaultp>
  sf::Packet& operator>>(sf::Packet& packet, glm::vec<L, T, Q>& vec) {
      for (glm::length_t i = 0; i < L; i++) {
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
}
