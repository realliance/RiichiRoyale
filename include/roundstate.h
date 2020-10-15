#pragma once
#include <map>
#include "piecetype.h"
#include "walls.h"
#include "enum.h"
#include "player.h"
#include "hand.h"

enum RoundStates{
  PlayerTurn,
  AfterKanPlayerTurn,
  KanDiscard,
  Discard
};

struct RoundState{
  Walls walls;
  std::map<Wind,int> players;
  Wind prevalentWind = East;
  int turnCount = 0;
  const Player* currentPlayer;
  int lastCall = -1;
  RoundStates currentState;
};
