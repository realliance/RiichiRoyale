#include "manager.h"

#include <stddef.h>            // for size_t
#include <algorithm>           // for find
#include <array>               // for array
#include <cstdint>             // for int16_t
#include <iostream>            // for operator<<, endl, cerr, ostream, basic...
#include <map>                 // for map
#include <memory>              // for allocator_traits<>::value_type
#include <string>              // for string
#include <vector>              // for vector
#include <thread>
#include <angrydiscardobot.h>
#include <pythonaiinterface.h>

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

auto MahjongGameManager::StartGame(std::vector<std::string> playerAIs, bool async) -> void {
  if(async){
    std::thread gameloop(&MahjongGameManager::GameLoop, playerAIs);
    gameloop.detach();
  }else{
    GameLoop(playerAIs);
  }
}

std::map<std::string,newMahjongAiInst> MahjongGameManager::availableAIs = {
  {"AngryDiscardoBot",[]() -> MahjongAI* {return new AngryDiscardoBot;}},
  {"PythonAIInterface",[]() -> MahjongAI* {return new PythonAIInterface;}}
};

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

auto MahjongGameManager::GameLoop(std::vector<std::string> seatAIs) -> void{
  GameState state;
  for(size_t i = 0; i < 4; i++){
    state.players[i].controller = availableAIs[seatAIs[i]]();
  }
  for(int i = 0; i < 4; i++){
    state.players[i].points = 25000;
    state.players[i].position = i;
    state.players[i].seat = static_cast<Wind>(i);
    state.players[i].controller->GameStart(i);
  }
  state.currentPlayer = 0;
  stateFunction newState = RoundStart;
  while(state.roundCounter < 8){
    newState = newState(state);
  }
  for(auto & player : state.players){
    player.controller->ReceiveEvent(END_EVENT);
    delete player.controller;
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
    state.hands[i] = Hand(hand);
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
    AlertPlayers(state,
      Event{
        Dora, // type
        -1, // player
        static_cast<int16_t>(state.walls.GetDoras().back().toUint8_t()), // piece
        false, // decision
      }
    );
    state.hands[state.currentPlayer].live.push_back(draw);
    state.hands[state.currentPlayer].sort();
  }else{
    draw = state.walls.TakePiece();
    state.hands[state.currentPlayer].live.push_back(draw);
    state.hands[state.currentPlayer].sort();
  }

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
  if(CanConvertedKan(state)){
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

  if(!needDecision){
    state.lastDiscard = draw;
    state.currentState = AfterDiscard;
    DiscardPiece(state, state.currentPlayer, draw);
    AlertPlayers(state, {
      Event{
        Discard, // type
        state.currentPlayer, // player
        static_cast<int16_t>(draw.toUint8_t()), // piece
        false, // decision
      }
    });
    return DiscardState;
  }

  Event decision;
  while(needDecision){
    decision = state.players[state.currentPlayer].controller->RetrieveDecision();
    needDecision = !ValidateDecision(state,state.currentPlayer, decision, true, draw);
  }

  if(decision.type == Tsumo){
    decision.piece = static_cast<int16_t>(draw.toUint8_t());
    state.winners.push_back(decision);
    AlertPlayers(state,decision);
    return RoundEnd;
  }


  if(decision.type == ConcealedKan){
    state.lastDiscard = decision.piece;
    decision.piece = static_cast<int16_t>(draw.toUint8_t());
    state.currentState = AfterConcealedKanDiscard;
    MeldPieces(state, state.currentPlayer, draw, ConcealedKan);
    AlertPlayers(state,decision);
    return DiscardState;
  }

  if(decision.type == ConvertedKan){
    state.lastDiscard = decision.piece;
    decision.piece = static_cast<int16_t>(draw.toUint8_t());
    state.currentState = AfterKanDiscard;
    MeldPieces(state, state.currentPlayer, draw, ConvertedKan);
    AlertPlayers(state,decision);
    return DiscardState;
  }



  if(decision.type == Discard){
    state.lastDiscard = decision.piece;
    state.currentState = AfterDiscard;
    DiscardPiece(state, state.currentPlayer, decision.piece);
    AlertPlayers(state,decision);
    return DiscardState;
  }

  if(decision.type == Riichi){
    state.lastDiscard = decision.piece;
    state.hands[state.currentPlayer].riichiRound = state.turnCount;
    state.hands[state.currentPlayer].riichiPieceDiscard = true;
    state.hands[state.currentPlayer].riichi = true;
    state.currentState = AfterRiichi;
    DiscardPiece(state, state.currentPlayer, decision.piece);
    AlertPlayers(state,decision);
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
      state.players[i].controller->ReceiveEvent(
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
      state.players[i].controller->ReceiveEvent(
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
      state.players[i].controller->ReceiveEvent(
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
      state.players[i].controller->ReceiveEvent(
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

  std::vector<Event> decision;
  decision.push_back(DECLINE_EVENT);
  for(int i = 0; i < 4; i++){
    while(needDecision[i]){
      Event tempDecision = state.players[i].controller->RetrieveDecision();
      if(ValidateDecision(state,i,tempDecision,false,state.lastDiscard)){
        needDecision[i] = false;
        if(tempDecision.type < decision[0].type){ // lower is higher priority
          tempDecision.player = i;
          tempDecision.piece = static_cast<int16_t>(state.lastDiscard.toUint8_t());
          decision[0] = tempDecision;
        }else if(tempDecision.type == Ron){
          tempDecision.player = i;
          tempDecision.piece = static_cast<int16_t>(state.lastDiscard.toUint8_t());
          decision.push_back(tempDecision);
        }
      }else{
        std::cerr << "Decision Validation Failed: " << tempDecision << std::endl;
      }
    }
  }
  if(decision[0].type == Decline){
    state.currentPlayer = (state.currentPlayer + 1) % 4;
    state.currentState = AfterDraw;
    state.turnCount++;
    return PlayerTurn;
  }
  
  if(decision[0].type == Ron){
    for(auto & d : decision){
      state.hands[d.player].live.push_back(decision[0].piece);
      state.winners.push_back(d);
      AlertPlayers(state,d);
    }
    return RoundEnd;
  }

  if(decision[0].type == Chi){
    state.currentPlayer = decision[0].player;
    state.hands[decision[0].player].live.push_back(decision[0].piece);
    state.hands[decision[0].player].sort();
    state.lastCall = state.turnCount;
    state.turnCount++;
    decision[0].piece = static_cast<int16_t>(GetChiStart(state,decision[0].player).toUint8_t());
    state.currentState = AfterCall;
    MeldPieces(state, decision[0].player, decision[0].piece, Chi);
    AlertPlayers(state,decision[0]);
    return PlayerTurn;
  }


  if(decision[0].type == Pon){
    state.hands[decision[0].player].open = true;
    state.hands[decision[0].player].live.push_back(decision[0].piece);
    state.hands[decision[0].player].sort();
    state.currentPlayer = decision[0].player;
    state.lastCall = state.turnCount;
    state.turnCount++;
    state.currentState = AfterCall;
    MeldPieces(state, decision[0].player, decision[0].piece, Pon);
    AlertPlayers(state,decision[0]);
    return PlayerTurn;
  }

  if(decision[0].type == Kan){
    state.hands[decision[0].player].open = true;
    state.currentPlayer = decision[0].player;
    state.hands[decision[0].player].live.push_back(decision[0].piece);
    state.hands[decision[0].player].sort();
    state.lastCall = state.turnCount;
    state.turnCount++;
    state.currentState = AfterKanDiscard;
    MeldPieces(state, decision[0].player, decision[0].piece, Kan);
    AlertPlayers(state,decision[0]);
    return DiscardState;
  }

  ErrorState(state, "ErrorState reached from DiscardState");
}

auto MahjongGameManager::RoundEnd(GameState& state) -> stateFunction {
  std::array<int16_t,4> scores = ScorePlayers(state);
  for(int i = 0; i < 4; i++){
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
  if(state.currentState == AfterConcealedKanDiscard || state.currentState == AfterKanDiscard ){
    return false;
  }
  if(CountPieces(state,player,state.lastDiscard) == 3){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanPon(const GameState& state, int player) -> bool{
  if(state.currentState == AfterConcealedKanDiscard || state.currentState == AfterKanDiscard ){
    return false;
  }
  if(CountPieces(state,player,state.lastDiscard) == 2){
    return true;
  }
  return false;
}

auto MahjongGameManager::GetChiStart(const GameState& state, int player) -> Piece{
  if(CountPieces(state,player,state.lastDiscard-2) > 0 && CountPieces(state,player,state.lastDiscard-1) > 0){
    return state.lastDiscard-2;
  }
  if(CountPieces(state,player,state.lastDiscard-1) > 0 && CountPieces(state,player,state.lastDiscard+1) > 0){
    return state.lastDiscard-1;
  }
  if(CountPieces(state,player,state.lastDiscard+1) > 0 && CountPieces(state,player,state.lastDiscard+2) > 0){
    return state.lastDiscard;
  }
  return ERROR_PIECE;
}

auto MahjongGameManager::CanChi(const GameState& state, int player) -> bool{
  if(state.currentState == AfterConcealedKanDiscard || state.currentState == AfterKanDiscard ){
    return false;
  }
  if(state.lastDiscard.isHonor()){
    return false;
  }
  if(((state.currentPlayer+1)%4) != player){
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

auto MahjongGameManager::CanTsumo(const GameState& state) -> bool{
  bool canTsumo = isComplete(state,state.currentPlayer);
  return canTsumo;
}

auto MahjongGameManager::CanConvertedKan(const GameState& state) -> bool{
  if(state.currentState == AfterCall){
    return false;
  }
  for(const auto & meld: state.hands[state.currentPlayer].melds){
    if(meld.type == PonMeld && CountPieces(state,state.currentPlayer,meld.start)==1){
      return true;
    }
  }
  return false;
}

auto MahjongGameManager::CanConcealedKan(const GameState& state, Piece p) -> bool{
  return CountPieces(state,state.currentPlayer,p) == 4;
}

auto MahjongGameManager::CanRiichi(const GameState& state) -> bool{
  return !state.hands[state.currentPlayer].riichi && isInAValidFormat(state,state.currentPlayer) == Tenpai ? true : false;
}

// Score given player
auto MahjongGameManager::ScorePlayers(const GameState& state) -> std::array<int16_t,4> {
  std::array<bool,4> isWinner = {false,false,false,false};
  std::array<int16_t,4> points = {0,0,0,0};
  for(const auto & winner : state.winners){
    int16_t handValue;
    if(state.currentState == AfterExhaustiveDraw){
      handValue = (30/state.winners.size())*(state.winners.size()<4);
    }else{
      handValue = 30; // placeholder for actual score calcuation later
    }
    points[winner.player] += handValue;
    points[state.currentPlayer] -= handValue;
    isWinner[winner.player] = true;
  }
  for(int i = 0; i < 4; i++){
    if(state.hands[i].riichi && !isWinner[i]){
      points[i] -= 10;
    }
  }
  return points;
}

// Push Event to Player Queue
auto MahjongGameManager::AlertPlayers(const GameState& state, Event e) -> void {
  e.decision = false;
  for(const auto & player : state.players){
    player.controller->ReceiveEvent(e);
  }
}

// Count number of piece p that are in given players hands
auto MahjongGameManager::CountPieces(const GameState& state, int player, Piece p) -> uint8_t {
  return std::count(state.hands[player].live.begin(),state.hands[player].live.end(),p);
}

// Remove an instance of piece p from given players hand
auto MahjongGameManager::RemovePieces(GameState& state, int player, Piece p, uint8_t count) -> uint8_t {
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
auto MahjongGameManager::DiscardPiece(GameState& state, int player, Piece p) -> void {
  RemovePieces(state,player,p,1);
  state.hands[player].discards.push_back(p);
}

// Produce a Meld given the Event
auto MahjongGameManager::MeldPieces(GameState& state, int player, Piece piece, EventType type) -> void {
  if (type == Pon) {
    if(RemovePieces(state,player,piece,3) != 3){
      ErrorState(state,"Not enough pieces to meld into a Pon: Player: " + std::to_string(player) + " Event: " + Piece(piece).toStr());
    }
    state.hands[player].melds.push_back({ PonMeld,  piece });
    return;
  }

  // Needs to be first piece given in event
  if (type == Chi) {
    if(RemovePieces(state,player,piece,1) != 1){
      ErrorState(state,"Not enough pieces to meld into a chi[0] player: " + std::to_string(player) + " Event: " + Piece(piece).toStr());
    }
    if(RemovePieces(state,player,piece+1,1) != 1){
      ErrorState(state,"Not enough pieces to meld into a chi[1]: " + std::to_string(player) + " Event: " + Piece(piece+1).toStr());
    }
    if(RemovePieces(state,player,piece+2,1) != 1){
      ErrorState(state,"Not enough pieces to meld into a chi[2]: " + std::to_string(player) + " Event: " + Piece(piece+2).toStr());
    }
    state.hands[player].melds.push_back({ ChiMeld, piece });
    return;
  }

  if (type == Kan) {
    if(RemovePieces(state,player,piece,4) != 4){
      ErrorState(state,"Not enough pieces to meld into a Kan: " + std::to_string(player) + " Event: " + Piece(piece).toStr());
    }
    state.hands[player].melds.push_back({ KanMeld,  piece });
    return;
  }

  if (type == ConcealedKan) {
    if(RemovePieces(state,player,piece,4) != 4){
      ErrorState(state,"Not enough pieces to meld into a ConcealedKan: " + std::to_string(player) + " Event: " + Piece(piece).toStr());
    }
    state.hands[player].melds.push_back({ ConcealedKanMeld, piece });
    return;
  }

  if (type == ConvertedKan) {
    if(RemovePieces(state,player,piece,1) != 1){
      ErrorState(state,"Not enough pieces to meld into a ConvertedKan: " + std::to_string(player) + " Event: " + Piece(piece).toStr());
    }
    for(auto& meld : state.hands[player].melds){
      if(meld.type == PonMeld && meld.start == piece){
        meld.type = KanMeld;
        return;
      }
    }
    ErrorState(state,"No meld to convert into a Kan: " + std::to_string(player) + " Event: " + Piece(piece).toStr());
  }
}

auto MahjongGameManager::ValidateDecision(GameState& state, int player, Event decision, bool inHand, Piece piece) -> bool {
  if(decision.type > Discard){
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
      return CanTsumo(state);
    case ConcealedKan:
      return CanConcealedKan(state,piece);
    case ConvertedKan:
      return CanConvertedKan(state);
    case Riichi:
      return CanRiichi(state) && CountPieces(state,player,decision.piece) > 0;
    case Discard:
      return CountPieces(state,player,decision.piece) > 0;
    case Decline:
      return true;
    default:
      ErrorState(state, "Default Case Reached in Validate Decision");
  }
}
