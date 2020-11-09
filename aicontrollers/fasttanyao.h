#pragma once
#include <string>
#include <vector>
#include <event.h>
#include <playercontroller.h>
#include <piecetype.h>
#include <winds.h>

/**
== Fast Tanyao Bot ==
- Attempt to get to all simples **quickly**
- Immediately dump all terminals and honors
- Call on everything that isn't a terminal or honor
- Disregard Defense (Where we're going, we don't need defense)
**/

using pieceSet = std::map<uint8_t, uint8_t>;

class FastTanyao : public Mahjong::PlayerController{
public:
  auto Name() -> std::string;
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void;
  auto ReceiveEvent(Mahjong::Event e) -> void;
  auto RetrieveDecision() -> Mahjong::Event;
private:
  auto IncrementPiece(Mahjong::Piece piece, pieceSet& set) -> void;
  auto DecrementPiece(Mahjong::Piece piece, pieceSet& set) -> void;
  auto ShouldKeep(Mahjong::Piece piece) -> bool;
  auto ProcessNewPiece(Mahjong::Piece piece) -> void;
  auto RecordDiscard(Mahjong::Piece piece) -> void;
  auto ChooseDiscard() -> Mahjong::Piece;
  pieceSet possibleTriples;
  std::vector<Mahjong::Piece> immediateDiscard;
  std::vector<Mahjong::Piece> validDoras;
  Mahjong::Event decidedDecision;
  pieceSet allDiscards;
};
