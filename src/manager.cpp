#include <stddef.h>
#include <string>
#include <vector>
#include <map>
class MahjongAI;

#include "manager.h"
#include "walls.h"
#include "enum.h"
#include "hand.h"

auto MahjongGameManager::StartGame(std::vector<std::string> playerAIs) -> int{
  for(size_t i = 0; i < 4; i++){
    boardState.hands.push_back(Hand(boardState.walls.TakeHand(i==0),static_cast<Wind>(i)));
    aiControllers.push_back(availableAIs[playerAIs[i]]());
  }
  boardState.wind = East;
  return 0;
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
  
}
