#pragma once
#include <stdint.h>         // for int16_t
#include <map>              // for map
#include <string>           // for string
#include <vector>           // for vector
#include "piecetype.h"      // for Piece
#include "statefunction.h"  // for stateFunction
class MahjongAI;  // lines 9-9
struct Event;  // lines 10-10
struct GameState;  // lines 11-11

using newMahjongAiInst = MahjongAI* (*)();

class MahjongGameManager{
  static std::map<std::string,newMahjongAiInst> availableAIs;
  static auto GameLoop(GameState& state) -> void;

  static auto RoundStart(GameState& state) -> stateFunction;
  static auto PlayerTurn(GameState& state) -> stateFunction;
  static auto DiscardState(GameState& state) -> stateFunction;
  static auto RoundEnd(GameState& state) -> stateFunction;

  _Noreturn static auto ErrorState(const GameState& state, std::string info) -> void;

  static auto RemovePieces(GameState& state, int player, Piece p, int count) -> Piece;
  static auto DiscardPiece(GameState& state, int player, Piece p) -> void;
  static auto MeldPieces(GameState& state, int player, Event e) -> void;

  static auto CanKan(const GameState& state, int player) -> bool;
  static auto CanPon(const GameState& state, int player) -> bool;
  static auto CanChi(const GameState& state, int player) -> bool;
  static auto GetChiStart(const GameState& state, int player) -> bool;
  static auto CanRon( GameState& state, int player) -> bool;

  static auto CanConvertedKan(const GameState& state, Piece p) -> bool;
  static auto CanConcealedKan(const GameState& state, Piece p) -> bool;
  static auto CanTsumo( GameState& state, Piece p) -> bool;
  static auto CanRiichi(const GameState& state) -> bool;


  static auto ScorePlayer(const GameState& state, int player) -> int16_t;
  static auto AlertPlayers(const GameState& state, Event e) -> void;
  static auto CountPieces(const GameState& state, int player, Piece p) -> int;
  static auto ValidateDecision(GameState& state, int player, Event decision, bool inHand, Piece p) -> bool;
public:  
  MahjongGameManager() = delete;
  static auto GetAvailableAIs() -> std::vector<std::string>;
  static auto RegisterAI(newMahjongAiInst newFunc, std::string Name) -> bool;
  static auto StartGame(std::vector<std::string> seatAIs) -> void;
};
