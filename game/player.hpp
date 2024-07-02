#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player {
  private:
    uintptr_t id;
    std::string token;
    std::string username;

  public:
    Player(uintptr_t id, std::string token, std::string username) {
      this->id = id;
      this->token = token;
      this->username = username;
    }
    
};

#endif
