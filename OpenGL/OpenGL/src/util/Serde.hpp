#pragma once

#include <sstream>

namespace Serde {

  template <typename T> class Binary {
  public:
    Binary() : val() {}
    Binary(T val) : val(val) {}
    T val;
  };

  template <typename T>
  std::ostream &operator<<(std::ostream &stream, Binary<T> &bin) {
    stream.write((char *)&bin.val, sizeof(T));
    return stream;
  }
  
  template <typename T>
  std::istream &operator>>(std::istream &stream, Binary<T> &bin) {
    stream.read((char *)&bin.val, sizeof(T));
    return stream;
  }

  std::string remainder(std::stringstream &ss);
};
