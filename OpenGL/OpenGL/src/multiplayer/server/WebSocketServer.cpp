// websockets stuff
#include "WebSocketServer.hpp"
#include "multiplayer/common/Config.hpp"

#include <SFML/Network.hpp>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

WebSocketServer* WebSocketServer::inst = nullptr;
bool WebSocketServer::stopSignal = false;


WebSocketServer::WebSocketServer(unsigned short port)
  : Server(port)
{
}

WebSocketServer::~WebSocketServer()
{}

void WebSocketServer::sigStop(int signal) {
  std::cout << "stopping WebSocket server" << std::endl;
  stopSignal = true;
  for(auto const& pair : inst->clientLookup) {
    auto con = inst->server.get_con_from_hdl(pair.second);
    con->close(websocketpp::close::status::no_status, "");
  }
  inst->server.stop_listening();
}

void WebSocketServer::loop() {
  sf::Clock clock;
  const sf::Time frameDuration = sf::milliseconds(NetworkConfig::SERVER_TICK);

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
  server.set_message_handler(bind(&WebSocketServer::on_message, this, _1, _2));
  server.set_open_handler(bind(&WebSocketServer::on_open, this, _1));
  server.set_close_handler(bind(&WebSocketServer::on_close, this, _1));
  server.set_access_channels(websocketpp::log::alevel::all);
  server.clear_access_channels(websocketpp::log::alevel::frame_header);
  server.clear_access_channels(websocketpp::log::alevel::frame_payload);
  server.set_error_channels(websocketpp::log::elevel::all);

  server.set_reuse_addr(true);
  server.init_asio();
  server.listen(port + 1);
  server.start_accept();

  mainThread = std::thread(&WebSocketServer::loop, this);

  server.run();
  std::cout << "stopping server.." << std::endl;
  mainThread.join();
}

ClientID WebSocketServer::htdl_to_client(websocketpp::connection_hdl hdl) {
  auto con = server.get_con_from_hdl(hdl);
  // sf::IpAddress ip = con->get_origin();
  sf::IpAddress ip;
  unsigned short port = con->get_port();
  return ClientID(ip, port);
}

websocketpp::connection_hdl WebSocketServer::client_to_hdl(ClientID client) {
  return clientLookup.find(client)->second;
}

void WebSocketServer::on_open(websocketpp::connection_hdl hdl) {
  ClientID client = htdl_to_client(hdl);
  clientLookup.emplace(client, hdl);

  // sf::Packet packet;
  // PacketHeader header(PacketType::LOGIN);
  // packet << header;
  // on_packet_recv(packet, client);
}

void WebSocketServer::on_close(websocketpp::connection_hdl hdl) {
  if(auto handle = hdl.lock()) {
    for (auto it = clientLookup.begin(); it != clientLookup.end(); ) {
      if(auto other = it->second.lock()) {
        if(other == handle) {
          it = clientLookup.erase(it);
        }
        else it++;
      }
    }
  }
  else {
    std::cout << "[WARN] WebSocket logout, but handle is already expired" << std::endl;
  }
}

void WebSocketServer::on_message(websocketpp::connection_hdl hdl, WebServer::message_ptr msg) {
  sf::Packet p;
  auto const& payload = msg->get_payload();
  p.append(payload.c_str(), payload.size());

  on_packet_recv(p, htdl_to_client(hdl));
}

void WebSocketServer::send(sf::Packet &packet, ClientID client) {
  auto it = clientLookup.find(client);
  if(it != clientLookup.end()) {
    server.send(it->second, packet.getData(), packet.getDataSize(), websocketpp::frame::opcode::binary);
  }
  else {
    std::cout << "[WARN] WebSocket client is missing: port " << client.getPort() << std::endl;
  }
}
