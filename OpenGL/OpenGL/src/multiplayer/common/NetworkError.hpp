#pragma once

#include <stdexcept>

class NetworkError : public std::runtime_error {

public:
  NetworkError(std::string msg);

private:

};
