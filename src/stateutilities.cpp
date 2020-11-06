#include "stateutilities.h"
#include <algorithm>
#include <vector>
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "player.h"
#include "decisionfunction.h"
#include "playercontroller.h"

#include "mahjongns.h"
using namespace Mahjong;

namespace Mahjong {

auto GetSeat(int round, int player) -> Wind{
  return static_cast<Wind>((player+3*(round%4))%4);
}

// Score given player
auto ScorePlayers(const GameState&) -> std::array<int16_t,4> {
  // std::array<bool,4> isWinner = {false,false,false,false};
  // std::array<int16_t,4> points = {0,0,0,0};
  // for(const auto & winner : state.winners){
  //   int16_t handValue;
  //   if(state.currentState == AfterExhaustiveDraw){
  //     handValue = (30/state.winners.size())*(state.winners.size()<4);
  //   }else{
  //     handValue = 30; // placeholder for actual score calcuation later
  //   }
  //   points[winner.player] += handValue;
  //   points[state.currentPlayer] -= handValue;
  //   isWinner[winner.player] = true;
  // }
  // for(int i = 0; i < 4; i++){
  //   if(state.hands[i].riichi && !isWinner[i]){
  //     points[i] -= 10;
  //   }
  // }
  // return points;
  return {0,0,0,0};
}

// Push Event to Player Queue
auto AlertPlayers(const GameState& state, Event e) -> void {
  e.decision = false;
  for(const auto & player : state.players){
    player.controller->ReceiveEvent(e);
  }
}

// Count number of piece p that are in given players hands
auto CountPieces(const GameState& state, int player, Piece p) -> uint8_t {
  return std::count(state.hands[player].live.begin(),state.hands[player].live.end(),p);
}

// Remove an instance of piece p from given players hand
auto RemovePieces(GameState& state, int player, Piece p, uint8_t count) -> uint8_t {
  count = std::min(CountPieces(state,player,p),count);
  uint8_t removed = 0;
  state.hands[player].live.erase(
    std::remove_if(state.hands[player].live.begin(), state.hands[player].live.end(),
      [&](Piece _p){
        if(count > removed && p == _p){
          removed++;
          return true;
        }
        return false;
      }
    ),
    state.hands[player].live.end()
  );
  return removed;
}

// Discard an instance of piece p from given players hand
auto DiscardPiece(GameState& state, int player, Piece p) -> void {
  RemovePieces(state,player,p,1);
  state.hands[player].discards.push_back(p);
}

auto AskForDiscard(const GameState& state) -> Piece {
  state.players[state.currentPlayer].controller->ReceiveEvent(
    Event{
      Event::Discard, // type
      state.currentPlayer, // player
      0, // piece
      true, // decision
    }
  );

  Event decision;
  bool valid = false;
  while(!valid){
    decision = state.players[state.currentPlayer].controller->RetrieveDecision();
    valid = ValidateDecision(state,state.currentPlayer, decision, true);
  }

  return decision.piece;
}

auto ValidateDecision(const GameState& state, int player, Event decision, bool inHand) -> bool {
  if(decision.type > Event::Discard){
    return false;
  }
  if(decision.type > Event::Decline && !inHand){
    return false;
  }
  if(decision.type < Event::Tsumo && inHand){
    return false;
  }
  switch (decision.type){
    case Event::Ron:
      return CanRon(state,player);
    case Event::Kan:
      return CanKan(state,player);
    case Event::Pon:
      return CanPon(state,player);
    case Event::Chi:
      return CanChi(state,player);
    case Event::Tsumo:
      return CanTsumo(state);
    case Event::ConcealedKan:
      return CanConcealedKan(state);
    case Event::ConvertedKan:
      return CanConvertedKan(state);
    case Event::Riichi:
      return CanRiichi(state) && CountPieces(state,player,decision.piece) > 0;
    case Event::Discard:
      return CountPieces(state,player,decision.piece) > 0;
    case Event::Decline:
      return true;
    default:
      return false;
  }
}

};
