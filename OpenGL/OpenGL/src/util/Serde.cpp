#include "Serde.hpp"

std::string Serde::remainder(std::stringstream &ss) {
  std::string s;
  std::string res;
  ss >> res;
  while (ss >> s) {
    res += " " + s;
  }
  return res;
}
