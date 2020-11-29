#include <array>
#include <vector>
#include <functional>
#include <random>
#include <thread>
#include "gamestate.h"
#include "controllermanager.h"
#include "player.h"
#include "settings.h"
#include "statefunctions.h"


#include "mahjongns.h"
using namespace Mahjong;

static int threadIndex = 1;
static std::map<int,bool> shouldHalt;

using namespace Mahjong;

auto Mahjong::StartGame(GameSettings settings, bool async) -> int {
  if(async){
    std::thread gameloop(&Mahjong::StateController, settings);
    gameloop.detach();
    return threadIndex;
  }
  StateController(settings);
  return 0;
}

auto Mahjong::ExitGame(int game) -> void{
  if(shouldHalt.contains(game)){
    shouldHalt[game] = true;
  }
}

auto Mahjong::StateController(GameSettings settings) -> void{
  GameState state;
  int id = threadIndex++;
  shouldHalt[id] = false;

  for(int i = 0; i < 4; i++){
    state.players[i].controller = GetController(settings.seatControllers[i])();
  }
  if(settings.seed){
    state.seed = settings.seed;
  }else{
    std::random_device rd;
    state.seed = rd();
  }
  if(!settings.overrideWall.empty()){
    std::swap(state.overrideWall, settings.overrideWall);
    state.seed = 0xBEEFBABE;
  }
  state.prevState = 0;
  state.nextState = GameStart;
  while(state.nextState != GameEnd && !shouldHalt[id]){
    try{
      state.prevState = state.nextState;
      state = state.nextState(state);
    } catch(const unsigned int e){
      switch (e)
      {
      case 0xFACEFEED: //Halted during controller decision
        shouldHalt.erase(id);
        return;
      case 0xBAD22222: //Asked for decision too many times.
        std::cerr << "Asked for decision too many times" << std::endl;
        state.nextState = Error;
        break;
      default:
        throw(e);
      }      
    }
  }
  if(state.nextState == GameEnd){
    state.nextState(state);
  }
}
