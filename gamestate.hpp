#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "game/player.hpp"

#include <mutex>
#include <unordered_map>

std::mutex mtx_connections;
std::unordered_map<uintptr_t, std::string> connections; // Map of connection IDs (ptr addresses) to tokens

std::mutex mtx_players;
std::unordered_map<std::string, Player> players; // Map of tokens to players

// std::mutex mtx_lobbies;
// std::unordered_map<std::string, std::string> lobbies; // Map of tokens to lobby IDs

#endif
