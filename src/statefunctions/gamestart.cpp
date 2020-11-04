#include <array>
#include "event.h"
#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
using namespace Mahjong;

auto Mahjong::GameStart(GameState& state) -> GameState& {
  std::cerr << "State: GameStart" << std::endl;
  for(int i = 0; i < 4; i++){
    state.players[i].points = 25000;
    state.players[i].controller->GameStart(i);
  }
  state.nextState = RoundStart;
  return state;
}
