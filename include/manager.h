#pragma once
#include <string>
#include <vector>
#include <map>
#include <array>

#include "gamestate.h"
struct Event;
class MahjongAI;

using newMahjongAiInst = MahjongAI* (*)();

class MahjongGameManager{
  GameState state;
  static std::map<std::string,newMahjongAiInst> availableAIs;
  auto GameLoop() -> void;
  auto RoundStart() -> void;
  auto PlayerDraw(int player) -> void;
  auto AvailableInHand(int player) -> std::vector<Event>;
  auto AvailableCalls(int player) -> std::vector<Event>;
  auto RemovePiece(int player, Piece p) -> bool;
  auto RoundEnd(int player, Event e) -> void;
  auto EventPriority(std::vector<Event> decisions) -> std::vector<Event>;
  auto ScoreHand(std::vector<Event> wins) -> std::array<int,4>;
  auto inline MahjongGameManager::CountPieces(int player, Piece p) const -> int;
  auto CanRon(int player, Event e) const -> bool;
  auto CanKan(int player, Event e) const -> bool;
  auto CanPon(int player, Event e) const -> bool;
  auto CanChi(int player, Event e) const -> bool;
  auto CanTsumo(int player, Event e) const -> bool;
  auto CanConcealedKan(int player, Event e) const -> bool;
  auto CanRiichi(int player, Event e) const -> bool;
public:  
  MahjongGameManager();
  static auto GetAvailableAIs() -> std::vector<std::string>;
  static auto RegisterAI(newMahjongAiInst newFunc, std::string Name) -> bool;
  auto StartGame(std::vector<std::string> seatAIs) -> void;
};
