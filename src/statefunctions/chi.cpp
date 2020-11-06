#include "statefunctions.h"
#include <stdint.h>
#include <array>
#include <vector>
#include "event.h"

#include "gamestate.h"
#include "hand.h"
#include "meld.h"
#include "stateutilities.h"
#include "piecetype.h"
#include "mahjongns.h"
using namespace Mahjong;

auto GetChiStart(const GameState& state, int player) -> Piece{
  if(CountPieces(state,player,state.pendingPiece-2) > 0 && CountPieces(state,player,state.pendingPiece-1) > 0){
    return state.pendingPiece-2;
  }
  if(CountPieces(state,player,state.pendingPiece-1) > 0 && CountPieces(state,player,state.pendingPiece+1) > 0){
    return state.pendingPiece-1;
  }
  if(CountPieces(state,player,state.pendingPiece+1) > 0 && CountPieces(state,player,state.pendingPiece+2) > 0){
    return state.pendingPiece;
  }
  return Piece::ERROR;
}

auto Mahjong::Chi(GameState& state) -> GameState& {  
  Piece chiStart = GetChiStart(state, state.lastCaller);
  if(chiStart == Piece::ERROR ){
    state.nextState = Error;
    return state;
  }

  state.hands[state.lastCaller].open = true;
  state.currentPlayer = state.lastCaller;

  AlertPlayers(state,Event{
    Event::Chi, // type
    state.lastCaller, // player
    static_cast<int16_t>(chiStart.toUint8_t()), // piece
    false, // decision
  });

  state.hands[state.lastCaller].live.push_back(state.pendingPiece);
  state.hands[state.lastCaller].sort();
  state.lastCall = state.turnNum;
  state.turnNum++;

  if(RemovePieces(state,state.lastCaller,chiStart,1) != 1 || RemovePieces(state,state.lastCaller,chiStart+1,1) != 1 || RemovePieces(state,state.lastCaller,chiStart+2,1) != 1){
    state.nextState = Error;
    return state;
  }
  state.hands[state.lastCaller].melds.push_back({ Meld::Chi, chiStart });

  state.pendingPiece = AskForDiscard(state);

  state.nextState = Discard;
  return state;
}
