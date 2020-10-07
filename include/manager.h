#pragma once
#include <string>
#include <vector>
#include <map>

#include "pieces.h"
#include "board.h"
class MahjongAI;

using newMahjongAiInst = MahjongAI* (*)();

class MahjongGameManager{
  Board boardState;
  static std::map<std::string,newMahjongAiInst> availableAIs;
  std::vector<MahjongAI*> aiControllers;
public:  
  static auto GetAvailableAIs() -> std::vector<std::string>;
  static auto RegisterAI(newMahjongAiInst newFunc, std::string Name) -> bool;
  using StartingWall = std::vector<Piece>;
  MahjongGameManager();
  auto StartGame(std::vector<std::string> seatAIs) -> int;
};
