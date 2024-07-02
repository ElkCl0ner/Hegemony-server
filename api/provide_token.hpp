#ifndef PROVIDE_TOKEN_HPP
#define PROVIDE_TOKEN_HPP

#define TOKEN_LENGTH 20

#include "../gamestate.hpp"
#include "utils/generate_token.hpp"

#include "../pch.hpp"

#include <chrono>
#include <random>

typedef websocketpp::server<websocketpp::config::asio> server;

/**
 * API endpoint: `provide_token`
 * Provide a token (reconnect to a session)
 * 
 * Expected payload:
 * - `token` (string): the token to validate
 * 
 * The server will:
 * 1. validate the format of the token
 *   a. valid: continue
 *   b. invalid: generate a new token
 * 2. assign the token to the client's connection ID
 * 3. send the token to the client via the `set_token` event
 * 4. check if the token is associated with a game session
 *   a. yes: send the current game state to the client via the `resynchronize` event
 *   b. no: end
 */
void api_provide_token(server *ws_server, websocketpp::connection_hdl hdl, nlohmann::json payload) {
  // Validate token
  bool valid_token = true;
  if (!payload.contains("token") || !payload["token"].is_string() || payload["token"].get<std::string>().length() != TOKEN_LENGTH) {
    valid_token = false;
  }
  std::string token = payload["token"];
  for (char c : token) {
    if (!(
      ('a' <= c && c <= 'z') ||
      ('A' <= c && c <= 'Z') ||
      ('0' <= c && c <= '9') ||
      c == '-'
    )) {
      valid_token = false;
    }
  }

  // Generate token if invalid
  if (!valid_token) {
    printf("Invalid token\n");
    token = generate_token(TOKEN_LENGTH);
  } else {
    printf("Valid token\n");
  }

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

  // Send game state to client
  // TODO: Implement this
}

#endif
