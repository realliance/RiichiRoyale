#include <stdint.h>
#include <array>
#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::Ron(GameState& state) -> GameState& {  
  //TODO scoring
  for(int player = 0; player < 4; player++){
    if(state.hasRonned[player]){
      AlertPlayers(state,Event{
        Event::Ron, // type
        player, // player
        static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
        false, // decision
      });
    }
  }
  state.nextState = RoundEnd;
  return state;
}
