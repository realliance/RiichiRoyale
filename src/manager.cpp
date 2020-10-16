#include "manager.h"

#include <stddef.h>      // for size_t
#include <stdint.h>      // for int16_t
#include <array>         // for array
#include <map>           // for map
#include <memory>        // for allocator_traits<>::value_type
#include <string>        // for string
#include <vector>        // for vector

#include "event.h"       // for Event, ConcealedKan, Discard, Dora, END_EVENT
#include "gamestate.h"   // for GameState
#include "hand.h"        // for Hand
#include "mahjongai.h"   // for MahjongAI
#include "piecetype.h"   // for Piece
#include "player.h"      // for Player
#include "roundstate.h"  // for RoundState, AfterDraw
#include "walls.h"       // for Walls
#include "winds.h"       // for Wind

auto MahjongGameManager::StartGame(std::vector<std::string> playerAIs) -> void {
  GameState state;
  for(size_t i = 0; i < 4; i++){
    state.players[i].controller = availableAIs[playerAIs[i]]();
  }
  GameLoop(state);
}

std::map<std::string,newMahjongAiInst> MahjongGameManager::availableAIs = {};

auto MahjongGameManager::GetAvailableAIs() -> std::vector<std::string> {
  std::vector<std::string> names;
  for(const auto &[name, _] : availableAIs){
    names.push_back(name);
  }
  return names;
}

auto MahjongGameManager::RegisterAI(newMahjongAiInst newFunc, std::string name) -> bool {
  if(availableAIs.contains(name)){
    return false;
  }
  availableAIs[name] = newFunc;
  return true;
}

auto MahjongGameManager::GameLoop(GameState& state) -> void{
  for(int i = 0; i < 4; i++){
    state.players[i].points = 25000;
    state.players[i].position = i;
    state.players[i].seat = static_cast<Wind>(i);
    state.players[i].controller->GameStart(i);
    state.currentRound.players.push_back(&state.players[i]);
  }
  state.currentRound.currentPlayer = 0;
  stateFunction newState = RoundStart;
  while(state.roundCounter < 8){
    newState(state.currentRound);
  }
  for(auto & player : state.players){
    player.controller->ReceiveEvent(END_EVENT);
  }
}

auto MahjongGameManager::RoundStart(RoundState& state) -> stateFunction {
  state.walls = Walls();
  for(size_t i = 0; i < 4; i++){
    auto hand = state.walls.TakeHand();
    state.players[i]->controller->RoundStart(
      hand,
      state.players[i]->seat,
      state.prevalentWind
    );
    state.hands.push_back(Hand(hand));
  }
  state.players[state.currentPlayer]->controller->ReceiveEvent(
    Event{
      Dora, // type
      -1, // player
      static_cast<int16_t>(state.walls.GetDoras()[0].toUint8_t()), // piece
      false, // decision
    }
  );
  return PlayerTurn;
}

auto MahjongGameManager::PlayerTurn(RoundState& state) -> stateFunction {
  Piece draw;
  if(state.currentState == AfterDraw){
    draw = state.walls.TakePiece();
  }else{
    draw = state.walls.TakeReplacementTile();
  }
  state.hands[state.currentPlayer].live.push_back(draw);
  state.hands[state.currentPlayer].sort();
  Event decision;
  bool needDecision = false;
  do{
    if(!state.hands[state.currentPlayer].riichi){
      state.players[state.currentPlayer]->controller->ReceiveEvent(
        Event{
          Discard, // type
          state.currentPlayer, // player
          static_cast<int16_t>(draw.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision = true;
    }
    if(CanConcealedKan(state)){
      state.players[state.currentPlayer]->controller->ReceiveEvent(
        Event{
          ConcealedKan, // type
          state.currentPlayer, // player
          static_cast<int16_t>(draw.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision = true;
    }
    if(CanConvertedKan(state)){
      state.players[state.currentPlayer]->controller->ReceiveEvent(
        Event{
          Kan, // type
          state.currentPlayer, // player
          static_cast<int16_t>(draw.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision = true;
    }
    if(CanRiichi(state)){
      state.players[state.currentPlayer]->controller->ReceiveEvent(
        Event{
          Riichi, // type
          state.currentPlayer, // player
          static_cast<int16_t>(draw.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision = true;
    }
    if(CanTsumo(state)){
      state.players[state.currentPlayer]->controller->ReceiveEvent(
        Event{
          Tsumo, // type
          state.currentPlayer, // player
          static_cast<int16_t>(draw.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision = true;
    }
    if(needDecision){
      decision = state.players[state.currentPlayer]->controller->RetrieveDecision();
    }
  }while(needDecision && ValidateDecision(state,state.currentPlayer, decision,true));

  if(!needDecision){
    DiscardPiece(state, state.currentPlayer, draw);
    AlertPlayers(state, {
      Event{
        Discard, // type
        state.currentPlayer, // player
        static_cast<int16_t>(draw.toUint8_t()), // piece
        false, // decision
      }
    });
    state.currentState = AfterDiscard;
    return DiscardState;
  }

  if(decision.type == Discard){
    DiscardPiece(state, state.currentPlayer, decision.piece);
    AlertPlayers(state,decision);
    state.currentState = AfterDiscard;
    return DiscardState;
  }

  if(decision.type == Riichi){
    state.hands[state.currentPlayer].riichiRound = state.turnCount;
    state.hands[state.currentPlayer].riichiPieceDiscard = true;
    state.hands[state.currentPlayer].riichi = true;
    DiscardPiece(state, state.currentPlayer, decision.piece);
    AlertPlayers(state,decision);
    state.currentState = AfterRiichi;
    return DiscardState;
  }

  if(decision.type == Kan){
    MeldPieces(state, state.currentPlayer, decision);
    AlertPlayers(state,decision);
    state.currentState = AfterKanDiscard;
    return DiscardState;
  }
}

// auto MahjongGameManager::EventPriority(std::vector<Event> decisions) -> std::vector<Event>{
//   // std::vector<Event> events;
//   // events.push_back(Event{Decline,Wind(-1),0});
//   // for(const auto & decision : decisions){
//   //   if(decision.type > events.front().type){
//   //     events.clear();
//   //     events.push_back(decision);
//   //   }else if(decision.type == events.front().type){
//   //     events.push_back(decision);
//   //   }
//   // }
//   // return events;
// }


// auto MahjongGameManager::CanRon(RoundState& state, int player, Piece p) -> bool{
//   // if(e.type != Discard && e.type != ConcealedKan && e.type != Kan){
//   //   return false;
//   // }
// }

// auto MahjongGameManager::CanKan(RoundState& state, int player, Piece p) -> bool{
//   //   if(e.type != Discard){
//   //   return false;
//   // }
//   // if(CountPieces(player,e.piece) == 3){
//   //   return true;
//   // }
//   // return false;
// }

// auto MahjongGameManager::CanPon(RoundState& state, int player, Piece p) -> bool{
//   // if(e.type != Discard){
//   //   return false;
//   // }
//   // if(CountPieces(player,e.piece) >= 2){
//   //   return true;
//   // }
//   // return false;
// }

// auto MahjongGameManager::CanChi(RoundState& state, int player, Piece p) -> bool{
//   // if(e.type != Discard || e.piece.isHonor()){
//   //   return false;
//   // }
//   // if(e.player-1 != player || !(player == 0 && e.player == 4)){
//   //   return false;
//   // }
//   // if(CountPieces(player,e.piece-2) > 0 && CountPieces(player,e.piece-1) > 0){
//   //   return true;
//   // }
//   // if(CountPieces(player,e.piece-1) > 0 && CountPieces(player,e.piece+1) > 0){
//   //   return true;
//   // }
//   // if(CountPieces(player,e.piece+1) > 0 && CountPieces(player,e.piece+2) > 0){
//   //   return true;
//   // }
// }

// auto MahjongGameManager::CanTsumo(RoundState& state) -> bool{
//   // if(e.type != Discard){
//   //   return false;
//   // }
//   // if(CountPieces(player, e.piece) == 0 && ){

//   // }
// }

// auto MahjongGameManager::CanConcealedKan(RoundState& state) -> bool{

// }

// auto MahjongGameManager::CanRiichi(RoundState& state) -> bool{

// }
