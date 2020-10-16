#pragma once
#include <map>
#include "piecetype.h"
#include "walls.h"
#include "player.h"
#include "hand.h"

enum RoundStates{
  AfterDraw,
  AfterReplacementDraw,
  AfterKanDiscard,
  AfterDiscard
};

struct RoundState{
  Walls walls;
  std::vector<Player*> players;
  std::vector<Hand> hands;
  Wind prevalentWind = East;
  int turnCount = 0;
  int currentPlayer = 0;
  int lastCall = -1;
  RoundStates currentState = AfterDraw;
};
