#include "event.h"
#include "gamestate.h"
#include "statefunctions.h"
using namespace Mahjong;

auto Mahjong::Exhaust(GameState& state) -> GameState& {  
  //TODO Score lmao
  state.nextState = RoundEnd;
  return state;
}
