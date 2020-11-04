#include "statefunctions.h"
#include <array>

#include "event.h"
#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
using namespace Mahjong;

auto Mahjong::GameEnd(GameState& state) -> GameState& {
  std::cerr << "State: GameEnd" << std::endl;
  for(auto & player : state.players){
    player.controller->ReceiveEvent(END_EVENT);
    delete player.controller;
  }
  return state;
}
