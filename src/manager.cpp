#include <stddef.h>
#include <string>
#include <vector>
#include <map>
class MahjongAI;

#include "manager.h"
#include "hand.h"

MahjongAIManager::MahjongAIManager(std::vector<startingHand> hands, std::vector<std::string> handAIs, Winds wind, Piece dora){
  for(size_t i = 0; i < 4; i++){
    boardState.hands.push_back(Hand(hands[i],static_cast<Winds>(i)));
    aiControllers.push_back(availableAIs[handAIs[i]]());
  }

  boardState.Dora.push_back(dora);
  boardState.Wind = wind;
}

std::map<std::string,newMahjongAiInst> MahjongAIManager::availableAIs = {};

auto MahjongAIManager::GetAvailableAIs() -> std::vector<std::string> {
  std::vector<std::string> names;
  for(const auto &[name, _] : availableAIs){
    names.push_back(name);
  }
  return names; 
}


auto MahjongAIManager::RegisterAI(newMahjongAiInst newFunc, std::string name) -> bool {
  if(availableAIs.contains(name)){
    return false;
  }
  availableAIs[name] = newFunc;
  return true;
}

