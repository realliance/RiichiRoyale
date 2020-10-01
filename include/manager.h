#pragma once
#include <string>
#include <vector>
#include <map>

#include "hand.h"
#include "pieces.h"
#include "board.h"
class MahjongAI;

using newMahjongAiInst = MahjongAI* (*)();

class MahjongAIManager{
  Board boardState;
  static std::map<std::string,newMahjongAiInst> availableAIs;
  std::vector<MahjongAI*> aiControllers;
public:  
  using startingHand = std::vector<Piece>;
  MahjongAIManager(std::vector<startingHand> hands, std::vector<std::string> handAIs, Winds wind, Piece dora);
  static auto GetAvailableAIs() -> std::vector<std::string>;
  static auto RegisterAI(newMahjongAiInst newFunc, std::string Name) -> bool;
};
