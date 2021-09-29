#include "UdpServer.hpp"
#include "../common/NetworkError.hpp"
#include "../common/Config.hpp"
#include "util/Identifier.hpp"
#include "debug/Debug.hpp"
#include "save/SaveManager.hpp"

bool UdpServer::stopSignal = false;

UdpServer::UdpServer(unsigned short port)
  : Server(port)
{

  socket.setBlocking(false);
  if(socket.bind(port) != sf::Socket::Done) {
    throw NetworkError("Failed to bind to port: " + std::to_string(port));
  }
}

UdpServer::~UdpServer()
{}

void UdpServer::run() {
  sf::Clock clock;
  const sf::Time frameDuration = sf::milliseconds(NetworkConfig::SERVER_TICK);

  std::signal(SIGINT, UdpServer::sigStop);

  while(!stopSignal) {
    sf::Time start = clock.getElapsedTime();

    while(poll()) {}
    on_server_tick();

    sf::Time elapsed = clock.getElapsedTime() - start;
    if(elapsed < frameDuration) {
      sf::sleep(frameDuration - elapsed);
    }
  }
}

bool UdpServer::poll() {
  sf::Packet packet;
  sf::IpAddress clientAddr;
  unsigned short clientPort;

  auto recv_res = socket.receive(packet, clientAddr, clientPort);

  if (recv_res != sf::Socket::Done) return false;
  on_packet_recv(packet, ClientID(clientAddr, clientPort));
  return true;
}

void UdpServer::sigStop(int signal) {
  stopSignal = true;
}

void UdpServer::send(sf::Packet& packet, ClientID client) {
  socket.send(packet, client.getAddr(), client.getPort());
}
