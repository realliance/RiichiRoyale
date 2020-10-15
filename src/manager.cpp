#include <stddef.h>
#include <string>
#include <array>
#include <vector>
#include <memory>
#include <map>

#include "event.h"
#include "piecetype.h"
#include "manager.h"
#include "walls.h"
#include "enum.h"
#include "mahjongai.h"
#include "hand.h"

auto MahjongGameManager::StartGame(std::vector<std::string> playerAIs) -> void {
  for(size_t i = 0; i < 4; i++){
    state.players[i].controller = availableAIs[playerAIs[i]]();
  }
  GameLoop();
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

auto MahjongGameManager::GameLoop() -> void{
  for(int i = 0; i < 4; i++){
    state.players[i].points = 25000;
    state.players[i].position = i;
    state.players[i].seat = static_cast<Wind>(i);
    state.players[i].controller->GameStart(i);
  }
  while(state.roundCounter < 8){
    RoundStart();
  }
  for(auto & player : state.players){
    player.controller->GameEnd();
  }
}

auto MahjongGameManager::RoundStart() -> void {
  state.walls = Walls();
  for(size_t i = 0; i < 4; i++){
    auto hand = state.walls.TakeHand();
    state.players[i].controller->RoundStart(hand,state.players[i].seat, state.wind);
    state.players[i].hand.live = hand;
  }
  state.turn = state.dealer;
  return PlayerTurn();
}

auto MahjongGameManager::PlayerTurn() -> void {
  state.players[state.turn].controller->ReceiveEvent(Event{Draw,state.turn,state.walls.TakePiece()});
  auto decisions = AvailableInHand(player);
  auto response = state.players[player].controller->MakeDecision(decisions);
  bool valid = false;
  switch(response.type){
    case Discard:
      valid = RemovePiece(player, response.piece);
      break;
    case Tsumo: {
      for(const auto & decision : decisions){
        if(decision.type == Tsumo){
          RoundEnd(player, response);
          break;
        }
      }
      break;
    }
    case ConcealedKan:
      

  }
}


auto MahjongGameManager::RoundLoop() -> void{


  std::vector<Event> current;
  current.push_back(Event{Draw,East,state.walls.TakePiece()});
  while(current[0].type != Ron && current[0].type != Tsumo){
    std::vector<Event> possibleEvents;
    auto decisions = AvailableDecisions();
    if(current[0].type == Draw){
      state.players[current[0].player].controller->ReceiveEvent(current[0]);
    }else if(current[0].type != Decline){
      for(int player = 0; player < 4; player++){
        state.players[player].controller->ReceiveEvent(current[0]);    
        if(decisions.contains(player)){
          possibleEvents.push_back(
            state.players[player].controller->MakeDecision(decisions[player])
          );
        } 
      }
    }
    current = EventPriority(possibleEvents);
  }
  auto breakdown = ScoreHand(current);
  for(int i = 0; i < 4; i++){
    state.players[i].controller->RoundEnd(breakdown);
  }
}

auto MahjongGameManager::AvailableDecisions() -> std::map<int,std::vector<Event>>{

}

auto MahjongGameManager::EventPriority(std::vector<Event> decisions) -> std::vector<Event>{
  std::vector<Event> events;
  events.push_back(Event{Decline,Wind(-1),0});
  for(const auto & decision : decisions){
    if(decision.type > events.front().type){
      events.clear();
      events.push_back(decision);
    }else if(decision.type == events.front().type){
      events.push_back(decision);
    }
  }
  return events;
}

auto MahjongGameManager::ScoreHand(std::vector<Event> wins) -> std::array<int,4>{

}


auto MahjongGameManager::CanRon(int player, Event e) const -> bool{
  if(e.type != Discard && e.type != ConcealedKan && e.type != Kan){
    return false;
  }
}

auto MahjongGameManager::CanKan(int player, Event e) const -> bool{
    if(e.type != Discard){
    return false;
  }
  if(CountPieces(player,e.piece) == 3){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanPon(int player, Event e) const -> bool{
  if(e.type != Discard){
    return false;
  }
  if(CountPieces(player,e.piece) >= 2){
    return true;
  }
  return false;
}

auto MahjongGameManager::CanChi(int player, Event e) const -> bool{
  if(e.type != Discard || e.piece.isHonor()){
    return false;
  }
  if(e.player-1 != player || !(player == 0 && e.player == 4)){
    return false;
  }
  if(CountPieces(player,e.piece-2) > 0 && CountPieces(player,e.piece-1) > 0){
    return true;
  }
  if(CountPieces(player,e.piece-1) > 0 && CountPieces(player,e.piece+1) > 0){
    return true;
  }
  if(CountPieces(player,e.piece+1) > 0 && CountPieces(player,e.piece+2) > 0){
    return true;
  }
}

auto MahjongGameManager::CanTsumo(int player, Event e) const -> bool{
  if(e.type != Discard){
    return false;
  }
  if(CountPieces(player, e.piece) == 0 && ){

  }
}

auto MahjongGameManager::CanConcealedKan(int player, Event e) const -> bool{

}

auto MahjongGameManager::CanRiichi(int player, Event e) const -> bool{

}
