#include <stdint.h>
#include <array>
#include <vector>
#include "decisionfunction.h"
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::PlayerHand(GameState& state) -> GameState&{
  std::cerr << "State: PlayerHand" << std::endl;
  using DecisionFunction = auto (*)(const Mahjong::GameState &state)->bool;

  struct possibleDecision{
    Event::Type type;
    DecisionFunction func;
  };

  std::vector<possibleDecision> decisions = {
    { Event::Tsumo, CanTsumo},
    { Event::ConcealedKan, CanConcealedKan},
    { Event::ConvertedKan, CanConvertedKan},
    { Event::Riichi, CanRiichi},
    { Event::Discard, [](const GameState& state){
        return !state.hands[state.currentPlayer].riichi;
      }
    }
  };

  bool decisionAsked = false;
  for(const auto& [decision, decisionIsPossible] : decisions){
    if(decisionIsPossible(state)){
      decisionAsked = true;
      state.players[state.currentPlayer].controller->ReceiveEvent(
        Event{
          decision, // type
          state.currentPlayer, // player
          static_cast<int16_t>(state.pendingPiece.toUint8_t()), // piece
          true, // decision
        }
      );
    }
  }

  if(!decisionAsked){
    state.nextState = Discard;
  }

  Event decision;
  while(decisionAsked){
    decision = state.players[state.currentPlayer].controller->RetrieveDecision();
    decisionAsked = !ValidateDecision(state,state.currentPlayer, decision, true, state.pendingPiece);
  }

  state.pendingPiece = decision.piece;

  switch (decision.type){
    case Event::Tsumo:
      state.nextState = Tsumo;
      break;
    case Event::ConcealedKan:
      state.nextState = ConcealedKan;
      break;
    case Event::ConvertedKan:
      state.nextState = ConvertedKan;
      break;
    case Event::Riichi:
      state.nextState = Riichi;
      break;
    case Event::Discard:
      state.nextState = Discard;
      break;
    default:
      state.nextState = Error;
      break;
  }

  return state;
}
