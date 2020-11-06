#include "statefunctions.h"
#include <iostream>
#include "gamestate.h"

#include "mahjongns.h"
using namespace Mahjong;

auto Mahjong::Error(GameState& state) -> GameState& {  
  std::cerr << "ERROR STATED REACHED" << std::endl;
  std::cerr << "GameState: " << std::endl;
  std::cerr << state;
  throw "ERROR STATE REACHED";
}
