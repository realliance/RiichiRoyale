#pragma once
#include <array>
#include "piecetype.h"
#include "walls.h"
#include "enum.h"
#include "hand.h"

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
  int dealer = 0;
  int counters = 0;
  int roundCounter = 0;
};
