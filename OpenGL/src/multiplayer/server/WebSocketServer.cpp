#include "WebSocketServer.hpp"

#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <openssl/ssl3.h>
#include <csignal>
#include <functional>
#include <iostream>
#include <utility>
#include "multiplayer/server/Server.hpp"
#include "save/ServerConfig.hpp"

using std::placeholders::_1;

static const rtc::Configuration config {

};

WebSocketServer* WebSocketServer::inst = nullptr;
bool WebSocketServer::stopSignal = false;

WebSocketServer::WebSocketServer(unsigned short port)
  : Server(port), server({ port })
{}

WebSocketServer::~WebSocketServer()
{}

void WebSocketServer::sigStop(int signal) {
  std::cout << "stopping WebSocket server" << std::endl;
  stopSignal = true;
}

void WebSocketServer::loop() {
  auto& config = Config::getServerConfig();
  sf::Clock clock;
  const sf::Time frameDuration = sf::milliseconds(config.webSocketServerTick);

  while(!stopSignal) {
    sf::Time start = clock.getElapsedTime();
    on_server_tick();
    sf::Time elapsed = clock.getElapsedTime() - start;

    if(elapsed < frameDuration) {
      sf::sleep(frameDuration - elapsed);
    }
  }
}

void WebSocketServer::run() {
  inst = this;
  std::signal(SIGINT, &WebSocketServer::sigStop);

  server.onClient(std::bind(&WebSocketServer::on_open, this, _1));
  std::cout << "listening on port " << server.port() << std::endl;

  // loop
  mainThread = std::thread(&WebSocketServer::loop, this);
  mainThread.join();
  
  // cleanup
  {
    std::lock_guard lck(allClientsLck);
    clientLookup.clear();
    server.stop();
  }

  std::cout << "stopped server" << std::endl;
}

void WebSocketServer::on_open(std::shared_ptr<rtc::WebSocket> socket) {
  std::cout << "[INFO] client login" << std::endl;
  ClientID client(socket->remoteAddress().value());

    auto peer = std::shared_ptr<Peer>(new Peer{
      .id = client,
      .conn = rtc::PeerConnection(config),
      .socket = socket,
      .channel = nullptr,
    });

    socket->onMessage(std::bind(&WebSocketServer::on_message, this, peer, _1));
    socket->onError([] (std::string e) { std::cerr << "[ERR] WebSocket error! " << e << std::endl; });
    socket->onClosed(std::bind(&WebSocketServer::on_close, this, peer));
    socket->onOpen([] () { std::cerr << "[INFO] WebSocket open" << std::endl; });

  	peer->conn.onLocalDescription([socket](rtc::Description description) { std::cout << "[INFO] Peer local description" << std::endl; socket->send(description); });
  	peer->conn.onLocalCandidate([socket](rtc::Candidate candidate) { std::cout << "[INFO] Peer Local candidate" << candidate << std::endl; socket->send(candidate); });
  	peer->conn.onStateChange([](rtc::PeerConnection::State state) { std::cout << "[INFO] Peer State: " << state << std::endl; });
  	peer->conn.onGatheringStateChange([](rtc::PeerConnection::GatheringState state) { std::cout << "[INFO] Peer Gathering State: " << state << std::endl; });
  	peer->conn.onDataChannel([this, peer](std::shared_ptr<rtc::DataChannel> channel) {
      std::lock_guard<std::mutex> lck(peer->mutex);
      std::cout << "[INFO] Peer Datachannel" << std::endl;
      peer->channel = channel;
      channel->onMessage(std::bind(&WebSocketServer::on_message, this, peer, _1));
      // channel->onClosed(std::bind(&WebSocketServer::on_close, this, peer));
      channel->onError([] (std::string e) { std::cerr << "[ERR] DataChannel error! " << e << std::endl; });
      channel->onOpen([] () { std::cout << "[INFO] DataChannel open" << std::endl; });
    });
    
    {
      std::lock_guard lck(allClientsLck);
      clientLookup.insert({ client, peer });
    }
}

void WebSocketServer::on_close(std::shared_ptr<Peer> peer) {
  std::cout << "[INFO] client logout" << std::endl;
  {
    std::lock_guard lck(allClientsLck);
    auto it = clientLookup.find(peer->id);
    if(it != clientLookup.end()) {
      clientLookup.erase(it);
    }
    else {
      std::cerr << "[ERR] logout of unknown client: " << peer->id.getAddr() << std::endl;
    }
  }
}

void WebSocketServer::on_message(std::shared_ptr<Peer> peer, rtc::message_variant msg) {
  // webrtc signaling
  if(std::holds_alternative<std::string>(msg)) {
    std::string data = std::get<std::string>(msg);
    if(data.starts_with("a=candidate")) {
      rtc::Candidate candidate(data, "MonCraft");
      std::cout << "[INFO] Peer Remote candidate: " << candidate << std::endl;
      std::lock_guard lck(peer->mutex);
      peer->conn.addRemoteCandidate(candidate);
    }
    else {
      rtc::Description description(data, rtc::Description::Type::Offer);
      std::cout << "[INFO] Peer Remote description" << std::endl;
      std::lock_guard lck(peer->mutex);
      peer->conn.setRemoteDescription(description);
    }
  }

  // actual real data, should generally go through the datachannel.
  else {
    sf::Packet p;
    auto data = std::get<std::vector<std::byte>>(msg);
    p.append(data.data(), data.size());

    on_packet_recv(p, peer->id);
  }
}

std::string get_password() {
  return "test";
}

std::shared_ptr<WebSocketServer::Peer> WebSocketServer::getPeer(ClientID peer) {
  std::lock_guard lck(allClientsLck);
  auto it = clientLookup.find(peer);
  if(it != clientLookup.end()) return it->second;
  else return nullptr;
  
}

bool WebSocketServer::send(sf::Packet &packet, ClientID client) {
  auto peer = getPeer(client);
  if (peer) {
    std::lock_guard<std::mutex> lck(peer->mutex);
    if(peer->channel && peer->channel->isOpen()) // prefer webrtc over websocket
      return peer->channel->send((std::byte*)packet.getData(), packet.getDataSize());
    else if(peer->socket->isOpen())
      return peer->socket->send((std::byte*)packet.getData(), packet.getDataSize());
    else
      return false;
  }
  else return false;
}
