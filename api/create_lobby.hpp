#ifndef CREATE_LOBBY_HPP
#define CREATE_LOBBY_HPP

// CONSIDER: Login system instead of no-login system
/**
 * API endpoint: `create_lobby`
 * Create a new lobby
 * 
 * Expected payload:
 * - `username` (string): the username of the client
 * 
 * The server will:
 * 1. generate a new lobby code
 * 2. create a new lobby with the lobby code
 * 3. send the lobby code to the client via the `set_lobby` event
 */
void api_create_lobby() {
  // TODO: check CONSIDER on ln 4 before implementing this
}

#endif
