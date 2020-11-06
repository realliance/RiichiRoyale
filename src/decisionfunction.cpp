#include "decisionfunction.h"
#include <array>
#include <algorithm>
#include <vector>
#include "hand.h"
#include "meld.h"
#include "gamestate.h"
#include "piecetype.h"
#include "stateutilities.h"
#include "hands.h"


using namespace Mahjong;

auto Mahjong::CanRon(const GameState& state, int player) -> bool{ //TODO: I really hate this
  GameState& tmpState = const_cast<GameState&>(state);
  tmpState.hands[player].live.push_back(state.pendingPiece);
  tmpState.hands[player].sort();
  // if(state.currentState == AfterConcealedKanDiscard){
  //   if(isThirteenOrphans(state,player)){
  //     return true;
  //   }else{
  //     return false;
  //   }
  // }
  bool canRon = isComplete(state,player);
  tmpState.hands[player].live.erase(
    std::find(state.hands[player].live.begin(),state.hands[player].live.end(),state.pendingPiece)
  );
  return canRon;
}

auto Mahjong::CanKan(const GameState& state, int player) -> bool{
  if(CountPieces(state,player,state.pendingPiece) == 3){
    return true;
  }
  return false;
}

auto Mahjong::CanPon(const GameState& state, int player) -> bool{
  if(CountPieces(state,player,state.pendingPiece) == 2){
    return true;
  }
  return false;
}

auto Mahjong::CanChi(const GameState& state, int player) -> bool{
  if(state.pendingPiece.isHonor()){
    return false;
  }
  if(((state.currentPlayer+1)%4) != player){
    return false;
  }
  if(CountPieces(state,player,state.pendingPiece-2) > 0 && CountPieces(state,player,state.pendingPiece-1) > 0){
    return true;
  }
  if(CountPieces(state,player,state.pendingPiece-1) > 0 && CountPieces(state,player,state.pendingPiece+1) > 0){
    return true;
  }
  if(CountPieces(state,player,state.pendingPiece+1) > 0 && CountPieces(state,player,state.pendingPiece+2) > 0){
    return true;
  }
  return false;
}

auto Mahjong::CanTsumo(const GameState& state) -> bool{
  bool canTsumo = isComplete(state,state.currentPlayer);
  return canTsumo;
}

auto Mahjong::CanConvertedKan(const GameState& state) -> bool{
  for(const auto & meld: state.hands[state.currentPlayer].melds){
    if(meld.type == Meld::Pon && CountPieces(state,state.currentPlayer,meld.start)==1){
      return true;
    }
  }
  return false;
}

auto Mahjong::CanConcealedKan(const GameState& state) -> bool{
  return CountPieces(state,state.currentPlayer,state.pendingPiece) == 4;
}

auto Mahjong::CanRiichi(const GameState& state) -> bool{
  return !state.hands[state.currentPlayer].riichi && isInAValidFormat(state,state.currentPlayer) == Tenpai ? true : false;
}
