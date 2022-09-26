#pragma once

#include <sstream>
#include <string>
#include "glm/glm.hpp"

namespace serde {

  template <typename T> class Binary {
  public:
    Binary() : val() {}
    Binary(T val) : val(val) {}
    T val;
  };

  template <typename T>
  std::ostream &operator<<(std::ostream &stream, Binary<T> const& bin) {
    stream.write((char *)&bin.val, sizeof(T));
    return stream;
  }
  
  template <typename T>
  std::istream &operator>>(std::istream &stream, Binary<T> &bin) {
    stream.read((char *)&bin.val, sizeof(T));
    return stream;
  }

  template<typename T, glm::length_t N, glm::qualifier Q>
  std::ostream& operator<<(std::ostream& stream, glm::vec<N, T, Q> const& v) {
    for(glm::length_t i = 0; i < N-1; i++)
      stream << v[i];
    return stream;
  }

  template<typename T, glm::length_t N, glm::qualifier Q>
  std::ostream& operator>>(std::ostream& stream, glm::vec<N, T, Q> &v) {
    for(glm::length_t i = 0; i < N-1; i++)
      stream >> v[i];
    return stream;
  }

  std::string remainder(std::stringstream &ss);
};
