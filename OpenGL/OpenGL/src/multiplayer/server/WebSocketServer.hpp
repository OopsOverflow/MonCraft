#pragma once

#include "Server.hpp"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


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
  typedef websocketpp::server<websocketpp::config::asio> WebServer;
  WebServer server;

  ClientID htdl_to_client(websocketpp::connection_hdl);
  websocketpp::connection_hdl client_to_hdl(ClientID);
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
