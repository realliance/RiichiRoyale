#pragma once
#include <iostream>


namespace Mahjong {
  class PlayerController;  

  struct Player {
    int points;
    PlayerController* controller = nullptr;
  };

};

auto operator<<(std::ostream& os, const Mahjong::Player& player) -> std::ostream&;
