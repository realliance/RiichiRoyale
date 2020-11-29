#include <array>
#include "event.h"
#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
using namespace Mahjong;

auto Mahjong::GameStart(GameState& state) -> GameState& {  
  for(int i = 0; i < 4; i++){
    state.players[i].points = 25000;
    state.players[i].controller->GameStart(i);
  }
  state.g.seed(state.seed);
  state.nextState = RoundStart;
  return state;
}
