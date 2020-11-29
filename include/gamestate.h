#pragma once
#include <vector>
#include <array>
#include <random>
#include <iostream>
#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "walls.h"

namespace Mahjong {

  struct GameState{
    bool halt = false;
    int currentPlayer = -1;
    int turnNum = -1;
    int roundNum = 0;
    int riichiSticks = 0;
    int counters = 0;
    int lastCall = -1;
    int lastCaller = -1;
    bool concealedKan = false;
    int seed = 0;
    std::mt19937 g;
    Piece pendingPiece = Piece::ERROR;
    auto (*nextState)(struct GameState&) -> struct GameState&;
    Walls walls;
    std::array<int,4> scores = {};
    std::array<bool,4> hasRonned = {};
    std::array<Hand,4> hands = {};
    std::array<Player,4> players = {};
    std::vector<Piece> overrideWall;
  };


};

auto operator<<(std::ostream& os, const Mahjong::GameState& state) -> std::ostream&;
