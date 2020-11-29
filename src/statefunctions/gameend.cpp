#include "statefunctions.h"
#include <array>

#include "event.h"
#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "mahjongns.h"
using namespace Mahjong;

auto Mahjong::GameEnd(GameState& state) -> GameState& {
  std::cout << "Scores: ";
  for(auto & player : state.players){
    player.controller->ReceiveEvent(END_EVENT);
    delete player.controller;
    std::cout << player.points << ", ";
  }
  std::cout << std::endl;
  return state;
}
