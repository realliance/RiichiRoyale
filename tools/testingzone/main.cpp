#include <fstream>

#include "analysis.h"
#include "handnode.h"
#include "hands.h"
#include "decisionfunction.h"
#include "walls.h"
#include "mahjongns.h"
#include "gamestate.h"
using namespace Mahjong;


int main(){
  std::vector<Piece> Hand = {
    Piece::ONE_CHARACTER,Piece::ONE_CHARACTER,Piece::TWO_PIN,Piece::THREE_PIN,
    Piece::THREE_PIN,Piece::FOUR_PIN,Piece::FOUR_PIN,Piece::FIVE_PIN
  };
  GameState state;
  state.roundNum = 0;
  state.currentPlayer = 0;
  state.hands[0].live = Hand;
  state.hands[0].melds = {
    {Meld::Chi,Piece::FIVE_CHARACTER},
    {Meld::Pon,Piece::RED_DRAGON}
  };
  Node* root = breakdownHand(Hand);
  std::cout << (root->IsComplete() ? "Complete" : "InComplete") << std::endl;
  for(const auto& branch : root->AsBranchVectors()){
    std::cout << (isWindOrDragonPon(state,0,branch) ? "yes yaku" : "no yaku") << std::endl;
  }
  return 0;
}
