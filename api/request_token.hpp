#ifndef REQUEST_TOKEN_HPP
#define REQUEST_TOKEN_HPP

#define TOKEN_LENGTH 20

#include "../gamestate.hpp"
#include "utils/generate_token.hpp"

#include "../pch.hpp"

#include <chrono>
#include <random>

typedef websocketpp::server<websocketpp::config::asio> server;

/**
 * API endpoint: `request_token`
 * Request a (new) token
 * 
 * The server will:
 * - generate a new token
 * - assign the token to the client's connection ID
 * - send the token to the client via the `set_token` event
 */
void api_request_token(server *ws_server, websocketpp::connection_hdl hdl, nlohmann::json payload) {
  // Generate token
  std::string token = generate_token(TOKEN_LENGTH);

  // Store token
  uintptr_t id = reinterpret_cast<uintptr_t>(hdl.lock().get());
  std::unique_lock<std::mutex> lock(mtx_connections);
  connections[id] = token;
  lock.unlock();
  printf("ID: %lu\tToken: %s\n", id, token.c_str());
  
  // Send token to client
  nlohmann::json response;
  response["event"] = "set_token";
  response["token"] = token;
  ws_server->send(hdl, response.dump(), websocketpp::frame::opcode::text);
}

#endif
