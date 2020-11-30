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
  // for(int i = 0; i < 100; i++){
  //   StartGame({
  //     { "Fast Tanyao","AngryDiscardoBot","GentlemanBot","ThriceBot"},
  //     {},
  //     0
  //   }, false);
  // }
  GameState state;
  state.nextState = Discard;
  state.seed = 0xBEEFBABE;
  state.g.seed(state.seed);
  state.walls = Walls(state.g);
  state.walls.TakeHand();state.walls.TakeHand();state.walls.TakeHand();state.walls.TakeHand();
  state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();state.walls.TakePiece();
  state.currentPlayer = 3;
  state.turnNum = 40;
  state.lastCall = 36;
  state.pendingPiece = Piece::NINE_CHARACTER;
  state.roundNum = 2;
  state.hands[0].live = {Piece::NINE_CHARACTER,Piece::NINE_CHARACTER,Piece::NINE_CHARACTER,Piece::ONE_PIN,Piece::TWO_PIN,Piece::THREE_PIN,Piece::EAST_WIND,Piece::EAST_WIND};
  state.hands[0].melds = {
    {
      Meld::Chi,
      Piece::THREE_BAMBOO
    },
    {
      Meld::Pon,
      Piece::SIX_BAMBOO
    }
  };

  std::cout << (isComplete(state,0) ? "Complete" : "Is Not Complete") << std::endl;


  return 0;
}
