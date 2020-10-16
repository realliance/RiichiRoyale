#include "manager.h"
#include <ext/alloc_traits.h>  // for __alloc_traits<>::value_type
#include <stddef.h>            // for size_t
#include <algorithm>           // for find
#include <array>               // for array
#include <cstdint>             // for int16_t
#include <iostream>            // for operator<<, endl, cerr, ostream, basic...
#include <map>                 // for map
#include <memory>              // for allocator_traits<>::value_type
#include <string>              // for string
#include <vector>              // for vector
#include "event.h"             // for Event, Kan, Discard, Chi, ConcealedKan
#include "gamestate.h"         // for GameState, AfterCall, AfterDraw, opera...
#include "hand.h"              // for Hand
#include "hands.h"             // for isComplete, isInAValidFormat, Tenpai
#include "mahjongai.h"         // for MahjongAI
#include "meld.h"              // for Meld, PonMeld
#include "piecetype.h"         // for Piece, ERROR_PIECE
#include "player.h"            // for Player
#include "walls.h"             // for Walls
#include "winds.h"             // for Wind, South

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
  }
  state.currentPlayer = 0;
  stateFunction newState = RoundStart;
  while(state.roundCounter < 8){
    newState(state);
  }
  for(auto & player : state.players){
    player.controller->ReceiveEvent(END_EVENT);
  }
}

auto MahjongGameManager::RoundStart(GameState& state) -> stateFunction {
  state.walls = Walls();
  for(size_t i = 0; i < 4; i++){
    auto hand = state.walls.TakeHand();
    state.players[i].controller->RoundStart(
      hand,
      state.players[i].seat,
      state.prevalentWind
    );
    state.hands.push_back(Hand(hand));
  }
  state.players[state.currentPlayer].controller->ReceiveEvent(
    Event{
      Dora, // type
      -1, // player
      static_cast<int16_t>(state.walls.GetDoras()[0].toUint8_t()), // piece
      false, // decision
    }
  );
  return PlayerTurn;
}

auto MahjongGameManager::PlayerTurn(GameState& state) -> stateFunction {
  if(state.walls.GetRemainingPieces() == 0){
    state.currentState = AfterExhaustiveDraw;
    return RoundEnd;
  }
  Piece draw;
  if(state.currentState == AfterCall){
    draw = state.lastDiscard;
  }else if(state.currentState == AfterReplacementDraw){
    draw = state.walls.TakeReplacementTile();
  }else{
    draw = state.walls.TakePiece();
  }
  state.hands[state.currentPlayer].live.push_back(draw);
  state.hands[state.currentPlayer].sort();

  bool needDecision = false;
  if(!state.hands[state.currentPlayer].riichi){
    state.players[state.currentPlayer].controller->ReceiveEvent(
      Event{
        Discard, // type
        state.currentPlayer, // player
        static_cast<int16_t>(draw.toUint8_t()), // piece
        true, // decision
      }
    );
    needDecision = true;
  }
  if(CanConcealedKan(state,draw)){
    state.players[state.currentPlayer].controller->ReceiveEvent(
      Event{
        ConcealedKan, // type
        state.currentPlayer, // player
        static_cast<int16_t>(draw.toUint8_t()), // piece
        true, // decision
      }
    );
    needDecision = true;
  }
  if(CanConvertedKan(state,draw)){
    state.players[state.currentPlayer].controller->ReceiveEvent(
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
    state.players[state.currentPlayer].controller->ReceiveEvent(
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
    state.players[state.currentPlayer].controller->ReceiveEvent(
      Event{
        Tsumo, // type
        state.currentPlayer, // player
        static_cast<int16_t>(draw.toUint8_t()), // piece
        true, // decision
      }
    );
    needDecision = true;
  }

  Event decision;
  while(needDecision){
    decision = state.players[state.currentPlayer].controller->RetrieveDecision();
    needDecision = !ValidateDecision(state,state.currentPlayer, decision,true);
  }

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
    state.lastDiscard = draw;
    state.currentState = AfterDiscard;
    return DiscardState;
  }

  AlertPlayers(state,decision);

  if(decision.type == Tsumo){
    return RoundEnd;
  }

  state.lastDiscard = decision.piece;

  if(decision.type == ConcealedKan){
    MeldPieces(state, state.currentPlayer, decision);
    state.currentState = AfterConcealedKanDiscard;
    return DiscardState;
  }

  if(decision.type == Kan){
    MeldPieces(state, state.currentPlayer, decision);
    state.currentState = AfterKanDiscard;
    return DiscardState;
  }

  DiscardPiece(state, state.currentPlayer, decision.piece);

  if(decision.type == Discard){
    state.currentState = AfterDiscard;
    return DiscardState;
  }

  if(decision.type == Riichi){
    state.hands[state.currentPlayer].riichiRound = state.turnCount;
    state.hands[state.currentPlayer].riichiPieceDiscard = true;
    state.hands[state.currentPlayer].riichi = true;

    state.currentState = AfterRiichi;
    return DiscardState;
  }

  std::cerr << "ErrorState reached from PlayerTurn" << std::endl;
  return ErrorState;
}



auto MahjongGameManager::DiscardState(GameState& state) -> stateFunction{
  std::array<bool,4> needDecision = {false,false,false,false};
  bool anyDecisionNeeded = false;
  for(int i = 0; i < 4; i++){
    if(i == state.currentPlayer){
      continue;
    }
    if(CanChi(state,i)){
      state.players[state.currentPlayer].controller->ReceiveEvent(
        Event{
          Chi, // type
          state.currentPlayer, // player
          static_cast<int16_t>(state.lastDiscard.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision[i] = true;
      anyDecisionNeeded = true;
    }
    if(CanPon(state,i)){
      state.players[state.currentPlayer].controller->ReceiveEvent(
        Event{
          Pon, // type
          state.currentPlayer, // player
          static_cast<int16_t>(state.lastDiscard.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision[i] = true;
      anyDecisionNeeded = true;
    }
    if(CanKan(state,i)){
      state.players[state.currentPlayer].controller->ReceiveEvent(
        Event{
          Kan, // type
          state.currentPlayer, // player
          static_cast<int16_t>(state.lastDiscard.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision[i] = true;
      anyDecisionNeeded = true;
    }
    if(CanRon(state,i)){
      state.players[state.currentPlayer].controller->ReceiveEvent(
        Event{
          Ron, // type
          state.currentPlayer, // player
          static_cast<int16_t>(state.lastDiscard.toUint8_t()), // piece
          true, // decision
        }
      );
      needDecision[i] = true;
      anyDecisionNeeded = true;
    }
  }


  if(!anyDecisionNeeded){
    state.currentPlayer = (state.currentPlayer + 1) % 4;
    state.currentState = AfterDraw;
    state.turnCount++;
    return PlayerTurn;
  }

  Event decision;
  decision.type = Decline;
  for(int i = 0; i < 4; i++){
    while(needDecision[i]){
      Event tempDecision = state.players[i].controller->RetrieveDecision();
      if(ValidateDecision(state,i,decision,false)){
        needDecision[i] = false;
        if(tempDecision.type < decision.type){ // lower is higher priority
          decision = tempDecision;
        }
      }
    }
  }
  if(decision.type == Decline){
    state.currentPlayer = (state.currentPlayer + 1) % 4;
    state.currentState = AfterDraw;
    state.turnCount++;
    return PlayerTurn;
  }
  
  AlertPlayers(state,decision);
  state.currentPlayer = decision.player;
  state.lastCall = state.turnCount;
  state.turnCount++;

  if(decision.type == Ron){
    return RoundEnd;
  }

  state.hands[decision.player].open = true;
  MeldPieces(state, state.currentPlayer, decision);

  if(decision.type == Chi){
    state.currentState = AfterCall;
    return PlayerTurn;
  }

  if(decision.type == Pon){
    state.currentState = AfterCall;
    return PlayerTurn;
  }

  if(decision.type == Kan){
    state.currentState = AfterKanDiscard;
    return DiscardState;
  }

  std::cerr << "ErrorState reached from DiscardState" << std::endl;

  return ErrorState;
}

auto MahjongGameManager::RoundEnd(GameState& state) -> stateFunction {
  std::vector<int16_t> scores;
  for(int i = 0; i < 4; i++){
    scores.push_back(ScorePlayer(state,i));
    state.players[i].points += scores[i];
  }
  if(state.currentState == AfterExhaustiveDraw){
    for(int i = 0; i < 4; i++){
      if(scores[i] > 0){
        state.winners.push_back(
          Event{
            ExhaustiveDraw, // type
            i, // player
            0, // piece
            false, // decision
          }
        );
        AlertPlayers(state, state.winners.back());
      }
    }
  }
  for(int i = 0; i < 4; i++){
    AlertPlayers(state, 
      Event{
        PointDiff, // type
        i, // player
        scores[i], // piece
        false, // decision
      }
    );
  }
  bool dealerWin = false;
  for(const auto & winner : state.winners){
    if(winner.player == state.dealer){
      dealerWin = true;
      break;
    }
  }
  if(!dealerWin){
    state.dealer = (state.dealer + 1) % 4;
    state.roundCounter++;
    if(state.roundCounter == 4){
      state.prevalentWind = South;
    }
  }
  for(int i = 0; i < 4; i++){
    if(scores[state.dealer] <= 0){
        state.players[i].seat = Wind((state.players[i].seat + 1) % 4);
    }
  }
  state.turnCount = 0;
  state.currentPlayer = state.dealer;
  state.lastCall = -1;
  state.lastDiscard = ERROR_PIECE;
  state.currentState = AfterDraw;
  state.hands.clear();
  return RoundStart;
}

auto MahjongGameManager::ErrorState(GameState& state) -> stateFunction {
  std::cerr << "ERROR STATED REACHED" << std::endl;
  std::cerr << "GameState: " << std::endl;
  std::cerr << state;
  throw "ERROR STATE REACHED";
}


auto MahjongGameManager::CanRon(GameState& state, int player) -> bool{
  state.hands[player].live.push_back(state.lastDiscard);
  state.hands[player].sort();
  if(isComplete(state,player)){
    return true;
  }
  state.hands[player].live.erase(
    std::find(state.hands[player].live.begin(),state.hands[player].live.end(),state.lastDiscard)
  );
  return false;
}

auto MahjongGameManager::CanKan(GameState& state, int player) -> bool{
  if(CountPieces(state,player,state.lastDiscard) == 3){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanPon(GameState& state, int player) -> bool{
  if(CountPieces(state,player,state.lastDiscard) == 2){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanChi(GameState& state, int player) -> bool{
  if(state.lastDiscard.isHonor()){
    return false;
  }
  if(state.currentPlayer-1 != player || !(player == 0 && state.currentPlayer == 4)){
    return false;
  }
  if(CountPieces(state,player,state.lastDiscard-2) > 0 && CountPieces(state,player,state.lastDiscard-1) > 0){
    return true;
  }
  if(CountPieces(state,player,state.lastDiscard-1) > 0 && CountPieces(state,player,state.lastDiscard+1) > 0){
    return true;
  }
  if(CountPieces(state,player,state.lastDiscard+1) > 0 && CountPieces(state,player,state.lastDiscard+2) > 0){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanTsumo(GameState& state) -> bool{
  state.hands[state.currentPlayer].live.push_back(state.lastDiscard);
  state.hands[state.currentPlayer].sort();
  if(isComplete(state,state.currentPlayer)){
    return true;
  }
  state.hands[state.currentPlayer].live.erase(
    std::find(
      state.hands[state.currentPlayer].live.begin(),
      state.hands[state.currentPlayer].live.end(),
      state.lastDiscard
    )
  );
  return false;
}

auto MahjongGameManager::CanConvertedKan(GameState& state, Piece p) -> bool{
  for(const auto & meld: state.hands[state.currentPlayer].melds){
    if(meld.type == PonMeld && meld.pieces[0] == p){
      return true;
    }
  }
  return false;
}

auto MahjongGameManager::CanConcealedKan(GameState& state, Piece p) -> bool{
  if(CountPieces(state,state.currentPlayer,p) == 4){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanRiichi(GameState& state) -> bool{
  return isInAValidFormat(state,state.currentPlayer) == Tenpai ? true : false;
}

// Score given player
auto MahjongGameManager::ScorePlayer(GameState& state, int player) -> int16_t {
  return 1000;
}

// Push Event to Player Queue
auto MahjongGameManager::AlertPlayers(GameState& state, Event e) -> void {
  state.players[e.player].controller->ReceiveEvent(e);
}

// Count number of piece p that are in given players hands
auto MahjongGameManager::CountPieces(GameState& state, int player, Piece p) -> int {
  uint8_t i = 0;
  for(auto& tile : state.hands[player].live) {
    if (tile == p) {
      i++;
    }
  }
  return i;
}

// Remove an instance of piece p from given players hand
auto MahjongGameManager::DiscardPiece(GameState& state, int player, Piece p) -> void {
  state.hands[player].live.erase(
    std::find(state.hands[player].live.begin(),state.hands[player].live.end(),p)
  );

  state.hands[player].discards.push_back(p);
}

// Produce a Meld given the Event
auto MahjongGameManager::MeldPieces(GameState& state, int player, Event e) -> void {
  if (e.type == Pon) {
    state.hands[player].melds.push_back({ PonMeld,  { e.piece, e.piece, e.piece } });
    return;
  }

  // Needs to be first piece given in event
  if (e.type == Chi) {
    state.hands[player].melds.push_back({ ChiMeld,  { e.piece, e.piece + 1, e.piece + 2 } });
    return;
  }

  if (e.type == Kan) {
    state.hands[player].melds.push_back({ KanMeld,  { e.piece, e.piece, e.piece, e.piece } });
    return;
  }

  if (e.type == ConcealedKan) {
    state.hands[player].melds.push_back({ ConcealedKanMeld,  { e.piece, e.piece, e.piece, e.piece } });
    return;
  }
}

// Is the event decision valid?
auto MahjongGameManager::ValidateDecision(GameState& state, int player, Event decision, bool inHand) -> bool {
  return true;

  // Complete Others
  
  if (decision.type == Ron) {
    if (inHand) {
      return false;
    }


  }
}