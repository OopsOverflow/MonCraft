#pragma once

// #include <websocketpp/config/asio.hpp>
// #include <websocketpp/server.hpp>
#include <chrono>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <rtc/rtc.hpp>

#include "Server.hpp"
#include "multiplayer/server/Client.hpp"

namespace sf { class Packet; }


/**
 * Quick and durty adapter for the usual SFML server to handle WebSockets connections.
 * more like a POC atm.
 * TODO: thread safety with handlers
 * TODO: overall rewrite
 */
class WebSocketServer : public Server {

public:
  WebSocketServer(unsigned short port);
  virtual ~WebSocketServer();

  void run() override;

protected:
  bool send(sf::Packet &packet, ClientID client) override;

private:
  rtc::WebSocketServer server;

  struct Peer {
    std::mutex mutex;
    ClientID id;
    rtc::PeerConnection conn;
    std::shared_ptr<rtc::WebSocket> socket;
    std::shared_ptr<rtc::DataChannel> channel;
  };

  ClientID sock_to_client(std::shared_ptr<rtc::WebSocket> socket);
  std::shared_ptr<rtc::WebSocket> client_to_sock(ClientID client);

  void on_message(Peer* peer, rtc::message_variant msg);
  void on_open(std::shared_ptr<rtc::WebSocket> socket);
  void on_close(Peer* peer);
  std::mutex clientLck;
  std::map<ClientID, std::unique_ptr<Peer>> clientLookup;

  std::thread mainThread;
  void loop();

  static bool stopSignal;
  static WebSocketServer* inst;
  static void sigStop(int signal);
};
