#include <stdint.h>
#include <array>
#include <vector>
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "hands.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"

using namespace Mahjong;



auto Mahjong::Riichi(GameState& state) -> GameState& {  
  //we should ask the players but
  // deadlines prevent this
  // -alice
  state.pendingPiece = getRiichiDiscard(state.hands[state.currentPlayer].live)[0];

  AlertPlayers(state,Event{
    Event::Riichi, // type
    state.currentPlayer, // player
    static_cast<int16_t>(Piece(state.pendingPiece).toUint8_t()), // piece
    false, // decision
  });

  state.hands[state.currentPlayer].riichiRound = state.turnNum;
  state.hands[state.currentPlayer].riichiPieceDiscard = state.hands[state.currentPlayer].discards.size();
  state.hands[state.currentPlayer].riichi = true;
  state.riichiSticks++;
  
  state.nextState = Discard;
  return state;
}
