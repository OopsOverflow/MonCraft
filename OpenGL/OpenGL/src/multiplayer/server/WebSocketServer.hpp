#pragma once

#include <websocketpp/config/asio.hpp>
#include <websocketpp/server.hpp>
#include <chrono>
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <thread>

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
  typedef websocketpp::server<websocketpp::config::asio_tls> WebServer;
  typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;
  WebServer server;

  enum tls_mode {
    MOZILLA_INTERMEDIATE = 1,
    MOZILLA_MODERN = 2
  };

  ClientID htdl_to_client(websocketpp::connection_hdl);
  websocketpp::connection_hdl client_to_hdl(ClientID);
  context_ptr on_tls_init();
  bool on_validate(websocketpp::connection_hdl);
  void on_message(websocketpp::connection_hdl, WebServer::message_ptr msg);
  void on_open(websocketpp::connection_hdl);
  void on_close(websocketpp::connection_hdl);
  std::map<ClientID, websocketpp::connection_hdl> clientLookup;

  std::thread mainThread;
  void loop();

  static bool stopSignal;
  static WebSocketServer* inst;
  static void sigStop(int signal);
};
