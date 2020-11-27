#include <fstream>

#include "analysis.h"
#include "handnode.h"
 
#include "mahjongns.h"
using namespace Mahjong;

int main(){
  std::vector<Piece> Hand = {
    Piece::TWO_BAMBOO,Piece::TWO_BAMBOO,Piece::TWO_BAMBOO,Piece::THREE_BAMBOO,
    Piece::THREE_BAMBOO,Piece::THREE_BAMBOO,Piece::FOUR_BAMBOO,Piece::FOUR_BAMBOO,
    Piece::FIVE_BAMBOO,Piece::FIVE_BAMBOO,Piece::SIX_BAMBOO,Piece::SIX_BAMBOO,
    Piece::SEVEN_BAMBOO,Piece::SEVEN_BAMBOO
  };

  Node* root = breakdownHand(Hand);
  std::ofstream os("hand.gv");
  root->DumpAsDot(os);
  os.close();
  auto branches = root->AsBranchVectors();
  for(const auto& branch : branches){
    std::cout << "Branch: " << std::endl;
    for(const auto& node : branch){
      std::cout << *node;
    }
    std::cout << std::endl;
  }
  delete root;
}
