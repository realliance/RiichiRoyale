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
  for(int i = 0; i < 1; i++){
    StartGame({
      { "Fast Tanyao","AngryDiscardoBot","GentlemanBot","ThriceBot"},
      {},
      0
    }, false);
  }
  return 0;
}
