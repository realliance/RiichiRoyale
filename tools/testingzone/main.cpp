#include <fstream>

#include "analysis.h"
#include "handnode.h"
#include "hands.h"
#include "decisionfunction.h"
#include "walls.h"
#include "mahjongns.h"
#include "gamestate.h"
#include "statefunctions.h"
using namespace Mahjong;


int main(){
  for(;;){
    StartGame({
      { "Fast Tanyao","Fast Tanyao","Fast Tanyao","Fast Tanyao"},
      {},
      0
    }, false);
  }
  return 0;
}
