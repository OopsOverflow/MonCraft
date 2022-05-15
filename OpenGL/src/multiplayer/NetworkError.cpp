#include "NetworkError.hpp"

NetworkError::NetworkError(std::string msg)
  : std::runtime_error("[NETWORK ERROR] " + msg)
{

}
