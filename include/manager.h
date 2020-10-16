#pragma once
#include <map>              // for map
#include <string>           // for string
#include <vector>           // for vector

#include "piecetype.h"      // for Piece
#include "statefunction.h"  // for stateFunction

class MahjongAI;  // lines 11-11
struct Event;  // lines 10-10
struct GameState;
struct RoundState;

using newMahjongAiInst = MahjongAI* (*)();

class MahjongGameManager{
  static std::map<std::string,newMahjongAiInst> availableAIs;
  static auto GameLoop(GameState& state) -> void;

  static auto RoundStart(RoundState& state) -> stateFunction;
  static auto PlayerTurn(RoundState& state) -> stateFunction;
  static auto KanState(RoundState& state) -> stateFunction;
  static auto TsumoState(RoundState& state) -> stateFunction;
  static auto RoundEnd(RoundState& state) -> stateFunction;
  static auto DiscardState(RoundState& state) -> stateFunction;
  static auto PonState(RoundState& state) -> stateFunction;
  static auto ChiState(RoundState& state) -> stateFunction;
  static auto RonState(RoundState& state) -> stateFunction;

  
  static auto EventPriority(std::vector<Event> decisions) -> std::vector<Event>;

  static auto CanRon(RoundState& state, int player, Piece p) -> bool;
  static auto CanKan(RoundState& state, int player, Piece p) -> bool;
  static auto CanPon(RoundState& state, int player, Piece p) -> bool;
  static auto CanChi(RoundState& state, int player, Piece p) -> bool;
  static auto CanTsumo(RoundState& state) -> bool;
  static auto CanConcealedKan(RoundState& state) -> bool;
  static auto CanRiichi(RoundState& state) -> bool;

  static auto CountPieces(RoundState& state, int player, Piece p) -> int;
  static auto DiscardPiece(RoundState& state, int player, Piece p) -> bool;
  static auto ValidateDecision(RoundState& state, int player, Event decision, bool inHand) -> bool;
public:  
  MahjongGameManager();
  static auto GetAvailableAIs() -> std::vector<std::string>;
  static auto RegisterAI(newMahjongAiInst newFunc, std::string Name) -> bool;
  static auto StartGame(std::vector<std::string> seatAIs) -> void;
};
