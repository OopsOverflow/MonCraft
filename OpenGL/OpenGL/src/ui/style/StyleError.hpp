#pragma once

#include <stdexcept>

class StyleError : public std::runtime_error {

public:
  StyleError(std::string const& what)
    : std::runtime_error(what)
  { }
};
