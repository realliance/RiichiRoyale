#include "statefunctions.h"
#include <array>

#include "event.h"
#include "gamestate.h"
#include "player.h"
#include "playercontroller.h"
#include "mahjongns.h"
#include "controllermanager.h"
using namespace Mahjong;

auto Mahjong::GameEnd(GameState& state) -> GameState& {
  std::cout << "Scores: ";
  for(auto & player : state.players){
    player.controller->ReceiveEvent(END_EVENT);
    std::cout << player.points << ", ";
    #ifndef NO_PYBIND
      if(pythonManagedControllers.contains(player.controller->Name())){
        continue;
      }
    #endif
    delete player.controller;
    player.controller = nullptr;
  }
  std::cout << std::endl;
  return state;
}
