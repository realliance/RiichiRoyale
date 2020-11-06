#include "statefunctions.h"
#include <array>
#include <vector>

#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "walls.h"
#include "mahjongns.h"
using namespace Mahjong;

auto Mahjong::Draw(GameState& state) -> GameState&{
  state.currentPlayer = (state.currentPlayer + 1) % 4;
  state.turnNum++;
  state.pendingPiece = state.walls.TakePiece();
  state.hands[state.currentPlayer].live.push_back(state.pendingPiece);
  state.hands[state.currentPlayer].sort();

  state.nextState = PlayerHand;
  return state;
}
