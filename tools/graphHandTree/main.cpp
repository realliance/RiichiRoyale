#include <fstream>

#include "possiblesets.h"
#include "handtree.h"
#include "handnode.h"
 
int main(){
  std::vector<Piece> Hand = GetPossibleStdFormHand();

  Node* root = breakdownHand(Hand);
  std::ofstream os("hand.gv");
  root->DumpAsDot(os);
  os.close();
  delete root;
}
