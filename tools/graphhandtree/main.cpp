#include <fstream>

#include "analysis.h"
#include "handnode.h"
 
#include "mahjongns.h"
using namespace Mahjong;

int main(){
  std::vector<Piece> Hand = GetPossibleStdFormHand();

  Node* root = breakdownHand(Hand);
  std::ofstream os("hand.gv");
  root->DumpAsDot(os);
  os.close();
  delete root;
}
