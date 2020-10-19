#pragma once
#include <iostream>  // for ostream
#include "winds.h"   // for Wind
class MahjongAI;  // lines 5-5

struct Player {
  int points;
  int position;
  Wind seat;
  MahjongAI* controller = nullptr;
};

auto operator<<(std::ostream& os, const Player& player) -> std::ostream&;
