#pragma once
#include <iostream>     // for ostream
#include <string>       // for allocator, string
#include <vector>       // for vector
#include "event.h"      // for Event
#include "hand.h"       // for Hand
#include "piecetype.h"  // for Piece, ERROR_PIECE
#include "player.h"     // for Player
#include "walls.h"      // for Walls
#include "winds.h"      // for East, Wind

enum GameStates{
  AfterDraw,
  AfterReplacementDraw,
  AfterKanDiscard,
  AfterConcealedKanDiscard,
  AfterDiscard,
  AfterRiichi,
  AfterCall,
  AfterExhaustiveDraw
};

struct GameState{
  Walls walls;
  std::vector<Player> players;
  std::vector<Hand> hands;
  std::vector<Event> winners;
  Wind prevalentWind = East;
  int turnCount = 0;
  int currentPlayer = 0;
  int lastCall = -1;
  int dealer = 0;
  int counters = 0;
  int roundCounter = 0;
  Piece lastDiscard = ERROR_PIECE;
  GameStates currentState = AfterDraw;
};

auto operator<<(std::ostream& os, const GameState& state) -> std::ostream&;

inline auto GameStateToStr(GameStates s) -> std::string{
  switch(s){
    case AfterDraw:
      return "AfterDraw";
    case AfterReplacementDraw:
      return "AfterReplacementDraw";
    case AfterKanDiscard:
      return "AfterKanDiscard";
    case AfterConcealedKanDiscard:
      return "AfterConcealedKanDiscard";
    case AfterDiscard:
      return "AfterDiscard";
    case AfterRiichi:
      return "AfterRiichi";
    case AfterCall:
      return "AfterCall";
    case AfterExhaustiveDraw:
      return "AfterExhaustiveDraw";
    default:
      return "InvalidState";

  }
}
