#pragma once
#include <array>
#include "piecetype.h"
#include "walls.h"
#include "hand.h"
#include "player.h"
#include "roundstate.h"

class MahjongAI;

struct GameState{
  std::array<Player,4> players;
  RoundState currentRound;
  Wind prevalentWind = East;
  int counters = 0;
  int roundCounter = 0;
};
