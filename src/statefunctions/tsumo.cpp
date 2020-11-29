#include <stdint.h>
#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "hands.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::Tsumo(GameState& state) -> GameState&{
  AlertPlayers(state,Event{
    Event::Tsumo, // type
    state.currentPlayer, // player
    static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
    false, // decision
  });
  int basicPoints = getBasicPoints(scoreHand(state,state.currentPlayer));
  state.scores[state.currentPlayer] += state.riichiSticks * 1000;
  state.riichiSticks = 0;
  state.scores[state.currentPlayer] += state.counters * 300;

  for(int i = 0; i < 4; i++){
    if(i == state.currentPlayer){
      if(state.hands[state.currentPlayer].riichi){
        state.scores[i] -= 1000;
      }
      continue;
    }
    int amount;
    if(state.currentPlayer == state.roundNum%4 || i == state.roundNum%4){
      amount = 2*basicPoints;
    }else{
      amount = basicPoints;
    }
    if(amount % 100){
      amount = amount+(100-(amount%100));
    }
    state.scores[i] -= amount;
    state.scores[i] -= state.counters * 100;
    if(state.hands[i].riichi){
      state.scores[i] -= 1000;
    }
    state.scores[state.currentPlayer] += amount;
  }


  if(state.currentPlayer == state.roundNum%4){
    state.counters++;
  }else{
    state.roundNum++;
    state.counters = 0;
  }
  state.nextState = RoundEnd;
  return state;
}
