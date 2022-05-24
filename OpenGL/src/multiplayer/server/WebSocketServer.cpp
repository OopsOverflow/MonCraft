// websockets stuff
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

WebSocketServer* WebSocketServer::inst = nullptr;
bool WebSocketServer::stopSignal = false;

WebSocketServer::WebSocketServer(unsigned short port)
  : Server(port), server({
      port = 16500,
    })
{}

WebSocketServer::~WebSocketServer()
{}

void WebSocketServer::sigStop(int signal) {
  std::cout << "stopping WebSocket server" << std::endl;
  stopSignal = true;
  // for(auto const& pair : inst->clientLookup) {
  //   auto con = inst->server.get_con_from_hdl(pair.second);
  //   con->close(websocketpp::close::status::no_status, "");
  // }
  // inst->server.stop_listening();
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

  server.onClient([this] (std::shared_ptr<rtc::WebSocket> socket) {
    std::cout << "client" << std::endl;

    on_open(socket);
    socket->onMessage(std::bind(&WebSocketServer::on_message, this, socket, _1));
    socket->onClosed(std::bind(&WebSocketServer::on_close, this, socket));
  });

  // server.set_validate_handler(bind(&WebSocketServer::on_validate, this, _1));
  // server.set_message_handler(bind(&WebSocketServer::on_message, this, _1, _2));
  // server.set_tls_init_handler(bind(&WebSocketServer::on_tls_init, this));
  // server.set_open_handler(bind(&WebSocketServer::on_open, this, _1));
  // server.set_close_handler(bind(&WebSocketServer::on_close, this, _1));
  // server.set_access_channels(websocketpp::log::alevel::all);
  // server.clear_access_channels(websocketpp::log::alevel::frame_header);
  // server.clear_access_channels(websocketpp::log::alevel::frame_payload);
  // server.set_error_channels(websocketpp::log::elevel::all);
  //
  // server.set_reuse_addr(true);
  // server.init_asio();
  // server.listen(port + 1);
  // server.start_accept();

  mainThread = std::thread(&WebSocketServer::loop, this);

  // server.run();
  // std::cout << "stopping server.." << std::endl;
  mainThread.join();
  std::cout << "stopped server" << std::endl;
}

ClientID WebSocketServer::sock_to_client(std::shared_ptr<rtc::WebSocket> socket) {
  // auto con = server.get_con_from_hdl(hdl);
  // sf::IpAddress ip;
  // unsigned short port = con->get_raw_socket().remote_endpoint().port();
  return ClientID(socket->remoteAddress().value());
}

std::shared_ptr<rtc::WebSocket> WebSocketServer::client_to_sock(ClientID client) {
  // client.getAddr().toString()
  return clientLookup.find(client)->second;
}
//
// bool WebSocketServer::on_validate(std::shared_ptr<rtc::WebSocket> hdl) {
//   auto con = server.get_con_from_hdl(hdl);
//   auto ps = con->get_requested_subprotocols();
//   con->select_subprotocol("binary");
//   return true;
// }
//
void WebSocketServer::on_open(std::shared_ptr<rtc::WebSocket> socket) {
  std::cout << "[INFO] client login" << std::endl;
  ClientID client = sock_to_client(socket);
  clientLookup.emplace(client, socket);
}

void WebSocketServer::on_close(std::shared_ptr<rtc::WebSocket> socket) {
  std::cout << "[INFO] client logout" << std::endl;
  // clients.erase(std::find(clients.begin(), clients.end(), socket));

  for (auto it = clientLookup.begin(); it != clientLookup.end(); ) {
    auto other = it->second;
    if(other == socket) {
      it = clientLookup.erase(it);
    }
    else it++;
  }
}

void WebSocketServer::on_message(std::shared_ptr<rtc::WebSocket> socket, rtc::message_variant msg) {
  std::cout << "on_message" << std::endl;
  sf::Packet p;
  auto data = std::get<std::vector<std::byte>>(msg);
  // auto const& payload = msg->get_payload();
  p.append(data.data(), data.size());

  on_packet_recv(p, sock_to_client(socket));
}

std::string get_password() {
  return "test";
}

// WebSocketServer::context_ptr WebSocketServer::on_tls_init() {
//   context_ptr ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
//
//   try {
//     auto opts = boost::asio::ssl::context::default_workarounds;
//     opts |= boost::asio::ssl::context::no_sslv2;
//     opts |= boost::asio::ssl::context::no_sslv3;
//     opts |= boost::asio::ssl::context::single_dh_use;
//     ctx->set_options(opts);
//
//     ctx->set_password_callback(bind(&get_password));
//     ctx->use_certificate_chain_file("cert/fullchain.pem");
//     ctx->use_private_key_file("cert/privkey.pem", boost::asio::ssl::context::pem);
//     ctx->use_tmp_dh_file("cert/dh.pem");
//
//     std::string ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK";
//
//     if (SSL_CTX_set_cipher_list(ctx->native_handle() , ciphers.c_str()) != 1) {
//       std::cout << "Error setting cipher list" << std::endl;
//     }
//
//   } catch (std::exception &e) {
//     std::cout << "Error in context pointer: " << e.what() << std::endl;
//   }
//
//   return ctx;
// }

void WebSocketServer::send(sf::Packet &packet, ClientID client) {
  auto it = clientLookup.find(client);
  if(it != clientLookup.end()) {
    auto res = it->second->send((std::byte*)packet.getData(), packet.getDataSize());
    if(!res) {
      std::cout << "[WARN] failed to send packet to client" << client.getAddr() << std::endl;
    }
  }
  else {
    std::cout << "[WARN] WebSocket client is missing: port " << client.getAddr() << std::endl;
  }
}
