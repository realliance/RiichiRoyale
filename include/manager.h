#pragma once
#include <string>
#include <vector>
#include <map>

#include "piecetype.h"
#include "gamestate.h"
#include "scorebreakdown.h"
struct Event;
class MahjongAI;

using newMahjongAiInst = MahjongAI* (*)();

class MahjongGameManager{
  GameState state;
  static std::map<std::string,newMahjongAiInst> availableAIs;
  auto GameLoop() -> void;
  auto RoundLoop() -> void;
  auto AvailableDecisions() -> std::map<int,std::vector<Event>>;
  auto EventPriority(std::vector<Event> decisions) -> std::vector<Event>;
  auto ScoreHand(std::vector<Event> wins) -> ScoreBreakdown;
public:  
  MahjongGameManager();
  static auto GetAvailableAIs() -> std::vector<std::string>;
  static auto RegisterAI(newMahjongAiInst newFunc, std::string Name) -> bool;
  auto StartGame(std::vector<std::string> seatAIs) -> void;
};
