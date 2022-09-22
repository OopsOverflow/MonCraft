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
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <utility>
#include "multiplayer/server/Server.hpp"
#include "save/ServerConfig.hpp"

using std::placeholders::_1;


static rtc::WebSocketServer::Configuration makeWSConf(unsigned short port, bool useTLS) {
  if (useTLS) return { port, true, "cert/pi.thissma.fr/cert.pem", "cert/pi.thissma.fr/privkey.pem" };
  else return { port };
}

static rtc::Configuration makeRTCConf() {
  rtc::Configuration config;
  config.iceServers.emplace_back("stun:stun.stunprotocol.org:3478");
  return config;
}

WebSocketServer* WebSocketServer::inst = nullptr;
bool WebSocketServer::stopSignal = false;

WebSocketServer::WebSocketServer(unsigned short port, bool tls)
  : Server(port), server(makeWSConf(port, tls))
{
  spdlog::info("Server config: port={}, tls={}", port, tls);
}

WebSocketServer::~WebSocketServer()
{}

void WebSocketServer::sigStop(int signal) {
  spdlog::info("Shutting down WebSocket/RTC server...");
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
  spdlog::info("Server listening on port {}", server.port());

  // loop
  mainThread = std::thread(&WebSocketServer::loop, this);
  mainThread.join();
  
  // cleanup
  {
    std::lock_guard lck(allClientsLck);
    clientLookup.clear();
    server.stop();
  }

  spdlog::info("Stopped server");
}

void WebSocketServer::on_open(std::shared_ptr<rtc::WebSocket> socket) {
  ClientID client(socket->remoteAddress().value());
  spdlog::info("Client login: {}", client.getAddr());

    auto peer = std::shared_ptr<Peer>(new Peer{
      .id = client,
      .conn = rtc::PeerConnection(makeRTCConf()),
      .socket = socket,
      .channel = nullptr,
    });

    socket->onMessage(std::bind(&WebSocketServer::on_message, this, peer, _1));
    socket->onError([peer] (std::string e) {
      spdlog::error("WebSocket error ({}): {}", peer->id.getAddr(), e);
    });
    socket->onClosed(std::bind(&WebSocketServer::on_close, this, peer));
    socket->onOpen([peer] () {
      spdlog::debug("WebSocket open ({})", peer->id.getAddr());
    });

    peer->conn.onLocalDescription([socket, peer](rtc::Description description) {
      spdlog::debug("RTC local description ({}): {}", peer->id.getAddr(), description);
      socket->send(description);
    });
    peer->conn.onLocalCandidate([socket, peer](rtc::Candidate candidate) {
      spdlog::debug("RTC local candidate ({}): {}", peer->id.getAddr(), candidate);
      socket->send(candidate);
    });

    peer->conn.onDataChannel([this, peer](std::shared_ptr<rtc::DataChannel> channel) {
      std::lock_guard<std::mutex> lck(peer->mutex);
      peer->channel = channel;
      channel->onMessage(std::bind(&WebSocketServer::on_message, this, peer, _1));
      channel->onClosed([peer] () {
        spdlog::debug("RTC closed ({})", peer->id.getAddr());
      });
      channel->onError([peer] (std::string e) {
        spdlog::error("RTC error ({}): {}", peer->id.getAddr(), e);
      });
      channel->onOpen([peer] () {
        spdlog::debug("RTC open ({})", peer->id.getAddr());
      });
    });
    
    {
      std::lock_guard lck(allClientsLck);
      clientLookup.insert({ client, peer });
    }
}

void WebSocketServer::on_close(std::shared_ptr<Peer> peer) {
  spdlog::info("Client logout: {}", peer->id.getAddr());
  {
    std::lock_guard lck(allClientsLck);
    auto it = clientLookup.find(peer->id);
    if(it != clientLookup.end()) {
      clientLookup.erase(it);
    }
    else {
      spdlog::error("Logout of unknown client: {}", peer->id.getAddr());
    }
  }
}

void WebSocketServer::on_message(std::shared_ptr<Peer> peer, rtc::message_variant msg) {
  // webrtc signaling
  if(std::holds_alternative<std::string>(msg)) {
    std::string data = std::get<std::string>(msg);
    if(data.starts_with("a=candidate")) {
      rtc::Candidate candidate(data, "");
      spdlog::debug("RTC Remote candidate ({}): {}", peer->id.getAddr(), data);
      std::lock_guard lck(peer->mutex);
      peer->conn.addRemoteCandidate(candidate);
    }
    else {
      rtc::Description description(data, rtc::Description::Type::Offer);
      spdlog::debug("RTC Remote description ({}): {}", peer->id.getAddr(), data);
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
