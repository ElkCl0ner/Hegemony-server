#include "api/request_token.hpp"
#include "api/provide_token.hpp"

#include "pch.hpp"

#include <iostream>
#include <thread>
#include <unordered_map>

typedef websocketpp::server<websocketpp::config::asio> server;


server ws_server; // WebSocket server instance

// Event handlers
std::unordered_map<std::string, std::function<void(server*, websocketpp::connection_hdl, nlohmann::json)>> event_handlers = {
  {"request_token", api_request_token},
  {"provide_token", api_provide_token},
};


void _on_open(websocketpp::connection_hdl hdl) {
  std::thread([](websocketpp::connection_hdl hdl) {
    // Print debug message
    std::cout << "Client connected" << std::endl;
  }, hdl).detach();
}


void _on_close(websocketpp::connection_hdl hdl) {
  std::thread([](websocketpp::connection_hdl hdl) {
    // Print debug message
    std::cout << "Client disconnected" << std::endl;
  }, hdl).detach();
}


void _on_message(websocketpp::connection_hdl hdl, server::message_ptr msg) {
  std::thread([](server *s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    // Print debug message
    std::cout << "Message received: " << msg->get_payload() << std::endl;

    // Parse JSON payload
    nlohmann::json payload;
    try {
      payload = nlohmann::json::parse(msg->get_payload());
    } catch (nlohmann::json::parse_error &e) {
      nlohmann::json error;
      error["event"] = "error";
      error["message"] = "Invalid JSON payload";
      s->send(hdl, error.dump(), websocketpp::frame::opcode::text);
      return;
    }

    // Get event
    if (!payload.contains("event")) { // Check if event is present
      nlohmann::json error;
      error["event"] = "error";
      error["message"] = "Event not specified";
      s->send(hdl, error.dump(), websocketpp::frame::opcode::text);
      return;
    }
    std::string event = payload["event"];

    // Handle event
    if (event_handlers.find(event) != event_handlers.end()) {
      event_handlers[event](s, hdl, payload);
    }
  }, &ws_server, hdl, msg).detach();
}


int main(int argc, char* argv[]) {
  // Set logging settings (non-verbose)
  ws_server.clear_access_channels(websocketpp::log::alevel::all);
  ws_server.clear_error_channels(websocketpp::log::elevel::all);

  // Initialize ASIO
  ws_server.init_asio();

  // Set up the server with the handlers
  ws_server.set_open_handler(_on_open);
  ws_server.set_close_handler(_on_close);
  ws_server.set_message_handler(_on_message);

  // Listen on port 8080
  ws_server.listen(8080);

  // Start the server accept loop
  ws_server.start_accept();

  // Start the ASIO io_service run loop
  ws_server.run();

  return 0;
}
