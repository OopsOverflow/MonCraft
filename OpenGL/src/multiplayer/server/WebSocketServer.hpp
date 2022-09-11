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
  
  
  std::shared_ptr<Peer> getPeer(ClientID id);

  void on_message(std::shared_ptr<Peer> peer, rtc::message_variant msg);
  void on_open(std::shared_ptr<rtc::WebSocket> socket);
  void on_close(std::shared_ptr<Peer> peer);

  std::mutex allClientsLck;
  std::map<ClientID, std::shared_ptr<Peer>> clientLookup;

  std::thread mainThread;
  void loop();

  static bool stopSignal;
  static WebSocketServer* inst;
  static void sigStop(int signal);
};
