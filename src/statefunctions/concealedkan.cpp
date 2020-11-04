#include <stdint.h>
#include <array>
#include <vector>
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "meld.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::ConcealedKan(GameState& state) -> GameState&{
  std::cerr << "State: ConcealedKan" << std::endl;
  AlertPlayers(state,Event{
    Event::ConcealedKan, // type
    state.currentPlayer, // player
    static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
    false, // decision
  });
  if(RemovePieces(state,state.currentPlayer,state.pendingPiece,4) != 4){
    state.nextState = Error;
  }
  state.hands[state.currentPlayer].melds.push_back({ Meld::ConcealedKan, state.pendingPiece });
  
  state.nextState = KanDiscard;
  return state;
}
