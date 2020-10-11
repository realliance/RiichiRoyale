#include <stddef.h>
#include <string>
#include <array>
#include <vector>
#include <map>

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
    RoundLoop();
  }
  for(auto & player : state.players){
    player.controller->GameEnd();
  }
}

auto MahjongGameManager::RoundLoop() -> void{
  state.walls = Walls();
  for(size_t i = 0; i < 4; i++){
    auto hand = state.walls.TakeHand(i==0);
    state.players[i].controller->RoundStart(hand,state.players[i].seat, state.wind);
    std::move(hand.begin(), hand.end(), state.players[i].hand.live);
  }

  std::vector<Event> current;
  current.push_back(Event{Decline});
  while(current[0].type != Ron && current[0].type != Tsumo){
    std::vector<Event> possibleEvents;
    auto decisions = AvailableDecisions();
    for(int i = 0; i < 4; i++){
      if(current[0].type != Decline){
        state.players[i].controller->ReceiveEvent(current[0]);
      }
      if(decisions.contains(i)){
        possibleEvents.push_back(
          state.players[i].controller->MakeDecision(decisions[i]));
      }
    }
    current = EventPriority(possibleEvents);
  }
  ScoreBreakdown breakdown = ScoreHand(state.players[current.player]);
  for(int i = 0; i < 4; i++){

  }
}