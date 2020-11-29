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
  std::vector<Piece> Hand = {Piece::ONE_CHARACTER,Piece::TWO_CHARACTER,Piece::THREE_CHARACTER,Piece::FOUR_CHARACTER,Piece::SIX_CHARACTER,Piece::EIGHT_CHARACTER,Piece::NINE_CHARACTER,
  Piece::ONE_PIN,Piece::FIVE_PIN,Piece::SIX_PIN,Piece::SEVEN_PIN,Piece::FOUR_CHARACTER,Piece::FIVE_CHARACTER,Piece::SIX_CHARACTER, Piece::SIX_CHARACTER};
  GameState state;
  state.currentPlayer = 0;
  state.hands[0].live = Hand;
  state.hands[0].riichi = false;
  state.hands[0].open = false;
  std::cout << (CanRiichi(state) ? "yes tenpai" : "no tenpai") << std::endl;
  return 0;
}
