#include <stdint.h>
#include <array>
#include "event.h"
#include "score.h"
#include "hands.h"
#include "gamestate.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::Ron(GameState& state) -> GameState& { 
  int basicPoints[4] = {};
  for(int player = 0; player < 4; player++){
    if(state.hasRonned[player]){
      AlertPlayers(state,Event{
        Event::Ron, // type
        player, // player
        static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
        false, // decision
      });
      basicPoints[player] = getBasicPoints(scoreHand(state,player));
    }
    if(state.hands[player].riichi){
      state.scores[player] -= 1000;
    }
  }
  int payment = 0;
  for(int i = 0; i < 4; i++){
    int player = (state.roundNum+i)%4;
    if(state.hasRonned[player]){
      state.scores[player] += 1000 * state.riichiSticks;
      state.riichiSticks = 0;
      state.scores[player] += 300*state.counters;
      payment += 300*state.counters;
      if(i == state.roundNum%4){
        int amount = 6*basicPoints[player];
        if(amount % 100){
          amount = amount+(100-(amount%100));
        }
        state.scores[player] += amount;
        payment += amount;
      }else{
        int amount = 4*basicPoints[player];
        if((amount % 100) != 0){
          amount = amount+(100-(amount%100));
        }
        state.scores[player] += amount;
        payment += amount;
      }
    }
  }

  state.scores[state.currentPlayer] -= payment;

  if(state.hasRonned[state.roundNum%4]){
    state.counters++;
  }else{
    state.roundNum++;
    state.counters = 0;
  }
  state.nextState = RoundEnd;
  return state;
}
