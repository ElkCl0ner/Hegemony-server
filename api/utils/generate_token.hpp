#ifndef GENERATE_TOKEN_HPP
#define GENERATE_TOKEN_HPP

#include <string>
#include <chrono>
#include <random>

/**
 * Generate a random token
 */
std::string generate_token(int token_length) {
  std::string token;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 61);
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  std::chrono::system_clock::duration epoch = now.time_since_epoch();
  long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
  token = std::to_string(ms) + "-";
  int num_chars = token_length - token.length();
  for (int i = 0; i < num_chars; i++) {
    int r = dis(gen);
    if (r < 26) {
      token += 'a' + r;
    } else if (r < 52) {
      token += 'A' + r - 26;
    } else {
      token += '0' + r - 52;
    }
  }

  return token;
}

#endif
