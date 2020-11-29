#include <stdint.h>
#include <array>
#include <vector>
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::Riichi(GameState& state) -> GameState& {  
  AlertPlayers(state,Event{
    Event::Riichi, // type
    state.currentPlayer, // player
    static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
    false, // decision
  });
  state.hands[state.currentPlayer].riichiRound = state.turnNum;
  state.hands[state.currentPlayer].riichiPieceDiscard = state.hands[state.currentPlayer].discards.size();
  state.hands[state.currentPlayer].riichi = true;
  state.riichiSticks++;
  
  state.nextState = Discard;
  return state;
}
