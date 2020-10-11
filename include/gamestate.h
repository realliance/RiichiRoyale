#pragma once
#include <array>
#include "pieces.h"
#include "walls.h"
#include "enum.h"

class MahjongAI;

struct Player {
  int points;
  int position;
  Wind seat;
  Hand hand;
  MahjongAI* controller;
};

struct GameState{
  Walls walls;
  std::array<Player,4> players;
  Wind wind = East;
  int counters = 0;
  int roundCounter = 0;
};
