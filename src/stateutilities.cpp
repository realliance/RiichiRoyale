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

  return GetValidDecisionOrThrow(state,state.currentPlayer, true).piece;
}

auto GetValidDecisionOrThrow(const GameState& state, int player, bool inHand) -> Event{
  Event decision;
  bool valid = false;
  int i = 0;
  while(!valid){
    if(i > 100){
      decision.type = inHand ? Event::Discard : Event::Decline;
      if(inHand){
        decision.piece = static_cast<int16_t>(state.pendingPiece.toUint8_t());
      }
      std::cerr << "WARNING: Player Controller sent invalid event too many times." << std::endl;
      if(ValidateDecision(state,player, decision, inHand)){
        return decision;
      }else{
        std::cerr << "ERROR: was not able to recover from invalid event." << std::endl;
        throw 0xBAD22222;
      }
    }
    i++;
    if(state.halt){
      throw 0xFACEFEED;
    }
    decision = state.players[player].controller->RetrieveDecision();
    valid = ValidateDecision(state,player, decision, inHand);
  }
  return decision;
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
