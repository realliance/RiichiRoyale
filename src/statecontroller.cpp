#include <array>
#include <string>
#include <vector>
#include <functional>
#include "gamestate.h"
#include "manager.h"
#include "player.h"
#include "statefunctions.h"


#include "mahjongns.h"
using namespace Mahjong;

auto Mahjong::StateController(std::vector<std::string> playerControllers) -> void{
  GameState state;
  for(int i = 0; i < 4; i++){
    state.players[i].controller = GetController(playerControllers[i])();
  }
  state.nextState = GameStart;
  while(state.nextState != GameEnd){
    state = state.nextState(state);
  }
  state.nextState(state);
}
