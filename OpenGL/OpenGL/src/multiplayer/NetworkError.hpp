#pragma once

#include <stdexcept>
#include <string>

class NetworkError : public std::runtime_error {

public:
  NetworkError(std::string msg);

private:

};
