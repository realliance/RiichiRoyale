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
        ConvertedKan, // type
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
  if(CanTsumo(state,draw)){
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
    needDecision = !ValidateDecision(state,state.currentPlayer, decision, true, draw);
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
    decision.piece = static_cast<int16_t>(draw.toUint8_t());
    MeldPieces(state, state.currentPlayer, decision);
    state.currentState = AfterConcealedKanDiscard;
    return DiscardState;
  }

  if(decision.type == ConvertedKan){
    decision.piece = static_cast<int16_t>(draw.toUint8_t());
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

  ErrorState(state, "ErrorState reached from PlayerTurn");
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
      if(ValidateDecision(state,i,decision,false,state.lastDiscard)){
        needDecision[i] = false;
        if(tempDecision.type < decision.type){ // lower is higher priority
          tempDecision.player = i;
          tempDecision.piece = static_cast<int16_t>(state.lastDiscard.toUint8_t());
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
  
  state.currentPlayer = decision.player;
  state.hands[decision.player].live.push_back(decision.piece);
  state.hands[decision.player].sort();
  state.lastCall = state.turnCount;
  state.turnCount++;

  if(decision.type == Chi){
    decision.piece = GetChiStart(state,decision.player);
    state.currentState = AfterCall;
    return PlayerTurn;
  }

  AlertPlayers(state,decision);


  if(decision.type == Ron){
    return RoundEnd;
  }

  state.hands[decision.player].open = true;
  MeldPieces(state, state.currentPlayer, decision);

  if(decision.type == Pon){
    state.currentState = AfterCall;
    return PlayerTurn;
  }

  if(decision.type == Kan){
    state.currentState = AfterKanDiscard;
    return DiscardState;
  }

  ErrorState(state, "ErrorState reached from DiscardState");
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

auto MahjongGameManager::ErrorState(const GameState& state, std::string info) -> void {
  std::cerr << "ERROR STATED REACHED" << std::endl;
  std::cerr << "INFO: " << info << std::endl;
  std::cerr << "GameState: " << std::endl;
  std::cerr << state;
  throw "ERROR STATE REACHED";
}


auto MahjongGameManager::CanRon(GameState& state, int player) -> bool{
  state.hands[player].live.push_back(state.lastDiscard);
  state.hands[player].sort();
  if(state.currentState == AfterConcealedKanDiscard){
    if(isThirteenOrphans(state,player)){
      return true;
    }else{
      return false;
    }
  }
  bool canRon = isComplete(state,player);
  state.hands[player].live.erase(
    std::find(state.hands[player].live.begin(),state.hands[player].live.end(),state.lastDiscard)
  );
  return canRon;
}

auto MahjongGameManager::CanKan(const GameState& state, int player) -> bool{
  if(CountPieces(state,player,state.lastDiscard) == 3){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanPon(const GameState& state, int player) -> bool{
  if(CountPieces(state,player,state.lastDiscard) == 2){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanChi(const GameState& state, int player) -> bool{
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

auto MahjongGameManager::CanTsumo(GameState& state, Piece p) -> bool{
  state.hands[state.currentPlayer].live.push_back(p);
  state.hands[state.currentPlayer].sort();
  bool canTsumo = isComplete(state,state.currentPlayer);
  state.hands[state.currentPlayer].live.erase(
    std::find(
      state.hands[state.currentPlayer].live.begin(),
      state.hands[state.currentPlayer].live.end(),
      p
    )
  );
  return canTsumo;
}

auto MahjongGameManager::CanConvertedKan(const GameState& state, Piece p) -> bool{
  for(const auto & meld: state.hands[state.currentPlayer].melds){
    if(meld.type == PonMeld && meld.start == p){
      return true;
    }
  }
  return false;
}

auto MahjongGameManager::CanConcealedKan(const GameState& state, Piece p) -> bool{
  return CountPieces(state,state.currentPlayer,p) == 4;
}

auto MahjongGameManager::CanRiichi(const GameState& state) -> bool{
  return isInAValidFormat(state,state.currentPlayer) == Tenpai ? true : false;
}

// Score given player
auto MahjongGameManager::ScorePlayer(const GameState& state, int player) -> int16_t {

}

// Push Event to Player Queue
auto MahjongGameManager::AlertPlayers(const GameState& state, Event e) -> void {
  for(const auto & player : state.players){
    player.controller->ReceiveEvent(e);
  }
}

// Count number of piece p that are in given players hands
auto MahjongGameManager::CountPieces(const GameState& state, int player, Piece p) -> int {
  return std::count(state.hands[player].live.begin(),state.hands[player].live.end(),p);
}

// Remove an instance of piece p from given players hand
auto MahjongGameManager::RemovePieces(GameState& state, int player, Piece p, int count) -> Piece {
  count = std::min(CountPieces(state,player,p),count);
  int deleteCnt = 0;
  state.hands[player].live.erase(
    std::remove_if(state.hands[player].live.begin(), state.hands[player].live.end(),
      [&](Piece _p){
        if(count > 0 && p == _p){
          count++;
          return true;
        }
      }
    ),
    state.hands[player].live.end()
  );
  return p;
}

// Discard an instance of piece p from given players hand
auto MahjongGameManager::DiscardPiece(GameState& state, int player, Piece p) -> void {
  state.hands[player].discards.push_back(RemovePieces(state,player,p,1));
}

// Produce a Meld given the Event
auto MahjongGameManager::MeldPieces(GameState& state, int player, Event e) -> void {
  if (e.type == Pon) {
    RemovePieces(state,player,e.piece,3);
    state.hands[player].melds.push_back({ PonMeld,  e.piece });
    return;
  }

  // Needs to be first piece given in event
  if (e.type == Chi) {
    RemovePieces(state,player,e.piece,1);
    RemovePieces(state,player,e.piece+1,1);
    RemovePieces(state,player,e.piece+2,1);
    state.hands[player].melds.push_back({ ChiMeld,  e.piece });
    return;
  }

  if (e.type == Kan) {
    RemovePieces(state,player,e.piece,4);
    state.hands[player].melds.push_back({ KanMeld,  e.piece });
    return;
  }

  if (e.type == ConcealedKan) {
    RemovePieces(state,player,e.piece,4);
    state.hands[player].melds.push_back({ ConcealedKanMeld,  e.piece });
    return;
  }

  if (e.type == ConvertedKan) {
    RemovePieces(state,player,e.piece,1);
    for(auto& meld : state.hands[player].melds){
      if(meld.type == PonMeld && meld.start == e.piece){
        meld.type = KanMeld;
      }
    }
    return;
  }
}

auto MahjongGameManager::ValidateDecision(GameState& state, int player, Event decision, bool inHand, Piece piece) -> bool {
  if(decision.type > Decline){
    return false;
  }
  if(decision.type > Decline && !inHand){
    return false;
  }
  if(decision.type < Tsumo && inHand){
    return false;
  }
  switch (decision.type){
    case Ron:
      return CanRon(state,player);
    case Kan:
      return CanKan(state,player);
    case Pon:
      return CanPon(state,player);
    case Chi:
      return CanChi(state,player);
    case Tsumo:
      return CanTsumo(state,piece);
    case ConcealedKan:
      return CanConcealedKan(state,piece);
    case ConvertedKan:
      return CanConvertedKan(state,piece);
    case Riichi:
      return CanRiichi(state);
    case Discard:
      return CountPieces(state,player,decision.piece) > 0;
    case Decline:
      return true;
    default:
      ErrorState(state, "Default Case Reached in Validate Decision");
  }
}
