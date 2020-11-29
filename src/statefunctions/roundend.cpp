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
  state.concealedKan = false;
  state.lastCaller = -1;
  state.pendingPiece = Piece::ERROR;
  state.hasRonned = {};
  
  state.hands = {};

  //TODO: scoring
  for(int i = 0; i < 4; i ++){
    AlertPlayers(state, Event{
      Event::PointDiff,
      i,
      static_cast<int16_t>(state.scores[i]/100),
      false
    });
    state.players[i].points += state.scores[i];
  }
  state.scores = {};

  std::cout << "ROUND NUM: " << state.roundNum << std::endl;
  if(state.roundNum > 3){ // east only lmao
    state.nextState = GameEnd;
  }else{
    state.nextState = RoundStart;
  }
  return state;
}
