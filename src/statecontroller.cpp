#include <array>
#include <vector>
#include <functional>
#include <random>
#include "gamestate.h"
#include "manager.h"
#include "player.h"
#include "settings.h"
#include "statefunctions.h"


#include "mahjongns.h"
using namespace Mahjong;

auto Mahjong::StateController(GameSettings settings) -> void{
  GameState state;
  for(int i = 0; i < 4; i++){
    state.players[i].controller = GetController(settings.seatControllers[i])();
  }
  if(settings.seed && settings.overrideWall.empty()){
    state.seed = settings.seed;
  }else{
    std::random_device rd;
    state.seed = rd();
  }
  if(!settings.overrideWall.empty()){
    std::swap(state.overrideWall, settings.overrideWall);
  }
  state.nextState = GameStart;
  while(state.nextState != GameEnd){
    state = state.nextState(state);
  }
  state.nextState(state);
}
