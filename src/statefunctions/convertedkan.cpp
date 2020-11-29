#include "statefunctions.h"
#include <stdint.h>
#include <array>
#include <vector>
#include "event.h"
#include "gamestate.h"

#include "hand.h"
#include "meld.h"
#include "piecetype.h"
#include "stateutilities.h"
#include "mahjongns.h"
using namespace Mahjong;

auto Mahjong::ConvertedKan(GameState& state) -> GameState&{
  AlertPlayers(state,Event{
    Event::ConvertedKan, // type
    state.currentPlayer, // player
    static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
    false, // decision
  });
  if(RemovePieces(state,state.currentPlayer,state.pendingPiece,1) != 1){
    std::cerr << "Not Enough pieces to remove in ConvertedKan" << std::endl;
    state.nextState = Error;
    return state;
  }
  state.concealedKan = false;
  for(auto& meld : state.hands[state.currentPlayer].melds){
    if(meld.type == Meld::Pon && meld.start == state.pendingPiece){
      meld.type = Meld::Kan;
      state.nextState = KanDiscard;
      return state;
    }
  }
  std::cerr << "Could Not find matching pon" << std::endl;
  state.nextState = Error;
  return state;
}
