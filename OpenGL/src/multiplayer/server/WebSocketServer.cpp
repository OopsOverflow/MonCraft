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

  mainThread = std::thread(&WebSocketServer::loop, this);

  mainThread.join();
  std::cout << "stopped server" << std::endl;
}

ClientID WebSocketServer::sock_to_client(std::shared_ptr<rtc::WebSocket> socket) {
  return ClientID(socket->remoteAddress().value());
}

std::shared_ptr<rtc::WebSocket> WebSocketServer::client_to_sock(ClientID client) {
  std::lock_guard<std::mutex> lck(clientLck);
  return clientLookup.find(client)->second->socket;
}

void WebSocketServer::on_open(std::shared_ptr<rtc::WebSocket> socket) {
  std::cout << "[INFO] client login" << std::endl;
  ClientID client = sock_to_client(socket);

    std::lock_guard<std::mutex> lck(clientLck);

    Peer* peer = new Peer{
      .id = sock_to_client(socket),
      .conn = rtc::PeerConnection(config),
      .socket = socket,
      .channel = nullptr,
    };

    socket->onMessage(std::bind(&WebSocketServer::on_message, this, peer, _1));
    socket->onClosed(std::bind(&WebSocketServer::on_close, this, peer));

  	peer->conn.onLocalDescription([socket](rtc::Description description) { socket->send(description); });
  	peer->conn.onLocalCandidate([socket](rtc::Candidate candidate) { socket->send(candidate); });
  	// peer.conn->onStateChange([](rtc::PeerConnection::State state) { std::cout << "[INFO] Peer State: " << state << std::endl; });
  	// peer.conn->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) { std::cout << "[INFO] Peer Gathering State: " << state << std::endl; });
  	peer->conn.onDataChannel([this, peer](std::shared_ptr<rtc::DataChannel> channel) {
      std::lock_guard<std::mutex> lck(peer->mutex);
      peer->channel = channel;
      channel->onMessage(std::bind(&WebSocketServer::on_message, this, peer, _1));
      channel->onClosed(std::bind(&WebSocketServer::on_close, this, peer));
    });

    clientLookup.insert({ client, std::unique_ptr<Peer>(peer) });
}

void WebSocketServer::on_close(Peer* peer) {
  std::cout << "[INFO] client logout" << std::endl;
  {
    std::lock_guard<std::mutex> lck(clientLck);
    auto it = clientLookup.find(peer->id);
    if(it != clientLookup.end()) {
      clientLookup.erase(it);
    }
    else {
      std::cerr << "[ERR] logout of unknown client: " << peer->id.getAddr() << std::endl;
    }
  }
}

void WebSocketServer::on_message(Peer* peer, rtc::message_variant msg) {
  // webrtc signaling
  if(std::holds_alternative<std::string>(msg)) {
    std::string data = std::get<std::string>(msg);
    if(data.starts_with("a=candidate")) {
      std::lock_guard<std::mutex> lck(peer->mutex);
      peer->conn.addRemoteCandidate(rtc::Candidate(data, "MonCraft"));
    }
    else {
      std::lock_guard<std::mutex> lck(peer->mutex);
      peer->conn.setRemoteDescription(rtc::Description(data, rtc::Description::Type::Answer));
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

bool WebSocketServer::send(sf::Packet &packet, ClientID client) {
  auto it = clientLookup.find(client);
  if(it != clientLookup.end()) {
    auto& peer = it->second;
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
