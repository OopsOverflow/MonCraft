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
  void send(sf::Packet &packet, ClientID client) override;

private:
  // typedef websocketpp::server<websocketpp::config::asio_tls> WebServer;
  // typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
  // WebServer server;
  rtc::WebSocketServer server;

  enum tls_mode {
    MOZILLA_INTERMEDIATE = 1,
    MOZILLA_MODERN = 2
  };

  ClientID sock_to_client(std::shared_ptr<rtc::WebSocket>);
  std::shared_ptr<rtc::WebSocket> client_to_sock(ClientID);
  // context_ptr on_tls_init();
  bool on_validate(std::shared_ptr<rtc::WebSocket>);
  void on_message(std::shared_ptr<rtc::WebSocket>, rtc::message_variant msg);
  void on_open(std::shared_ptr<rtc::WebSocket>);
  void on_close(std::shared_ptr<rtc::WebSocket>);
  std::map<ClientID, std::shared_ptr<rtc::WebSocket>> clientLookup;

  std::thread mainThread;
  void loop();

  static bool stopSignal;
  static WebSocketServer* inst;
  static void sigStop(int signal);
};
