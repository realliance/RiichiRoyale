#include <stdint.h>
#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::Tsumo(GameState& state) -> GameState&{
  
  //TODO scoring
  AlertPlayers(state,Event{
    Event::Tsumo, // type
    state.currentPlayer, // player
    static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
    false, // decision
  });
  state.nextState = RoundEnd;
  return state;
}
