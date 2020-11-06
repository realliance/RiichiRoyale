#include <stdint.h>
#include <array>
#include <memory>
#include <vector>
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
#include "walls.h"
using namespace Mahjong;

auto Mahjong::Replacement(GameState& state) -> GameState& {  
  Piece draw = state.walls.TakeReplacementTile();
  state.hands[state.currentPlayer].live.push_back(draw);
  state.hands[state.currentPlayer].sort();
  state.pendingPiece = draw;

  AlertPlayers(state,Event{
    Event::Dora, // type
    -1, // player
    static_cast<int16_t>(state.walls.GetDoras().back().toUint8_t()), // piece
    false, // decision
  });

  state.nextState = PlayerHand;
  return state;
}
