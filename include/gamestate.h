#pragma once
#include <vector>
#include <iostream>
#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "walls.h"

namespace Mahjong {

  struct GameState{
    int currentPlayer = -1;
    int turnNum = -1;
    int roundNum = 0;
    int riichiSticks = 0;
    int counters = 0;
    int lastCall = -1;
    int lastCaller = -1;
    int seed = 0;
    Piece pendingPiece;
    auto (*nextState)(struct GameState&) -> struct GameState&;
    Walls walls;
    std::array<bool,4> hasRonned;
    std::array<Hand,4> hands;
    std::array<Player,4> players;
    std::vector<Piece> overrideWall;
  };


};

auto operator<<(std::ostream& os, const Mahjong::GameState& state) -> std::ostream&;
