#include <array>
#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::RoundEnd(GameState& state) -> GameState& {  
  state.currentPlayer = -1;
  state.turnNum = -1;
  state.lastCall = -1;
  state.lastCaller = -1;
  state.pendingPiece = Piece::ERROR;
  state.hasRonned = { false, false, false, false };

  //TODO: scoring
  for(int i = 0; i < 4; i ++){
    AlertPlayers(state, Event{
      Event::PointDiff,
      i,
      0,
      false
    });
  }
  
  state.nextState = RoundStart;
  return state;
}
