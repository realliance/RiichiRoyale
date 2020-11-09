#include "fasttanyao.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind
#include <iostream>     // for endl, ostream, basic_ostream<>::__ostream_type
#include <memory>       // for allocator_traits<>::value_type
#include <tuple>
#include <algorithm>

auto FastTanyao::Name() -> std::string{
  return "Fast Tanyao";
}

auto FastTanyao::GameStart(int) -> void {}

auto FastTanyao::ShouldKeep(Mahjong::Piece piece) -> bool {
  return !piece.isHonor() && piece + 1 != Mahjong::Piece::ERROR && !(piece + 1).isTerminal();
}

auto FastTanyao::IncrementPiece(Mahjong::Piece piece, pieceSet& set) -> void {
  pieceSet::iterator setContainsPiece = set.find(piece.raw_value());
  if(setContainsPiece != set.end()) {
    setContainsPiece->second++;
    return;
  }

  set.insert(std::pair<uint8_t, uint8_t>(piece.raw_value(), 1));
}

auto FastTanyao::DecrementPiece(Mahjong::Piece piece, pieceSet& set) -> void {
  pieceSet::iterator setContainsPiece = set.find(piece.raw_value());
  if(setContainsPiece != set.end()) {
    setContainsPiece->second--;
    if (setContainsPiece->second == 0) {
      set.erase(setContainsPiece);
    }
    return;
  }
}

auto FastTanyao::ProcessNewPiece(Mahjong::Piece piece) -> void {
  if (!ShouldKeep(piece)) {
    immediateDiscard.push_back(piece);
    return;
  }

  IncrementPiece(piece, possibleTriples);
}

auto FastTanyao::RecordDiscard(Mahjong::Piece piece) -> void {
  if (ShouldKeep(piece)) {
    IncrementPiece(piece, allDiscards);
  }
}

auto FastTanyao::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind _seatWind, Mahjong::Wind _prevalentWind) -> void {
  for (auto piece : _hand) {
    ProcessNewPiece(piece);
  }
  decidedDecision.type = Mahjong::Event::Discard;
}

auto FastTanyao::ChooseDiscard() -> Mahjong::Piece {
  if (immediateDiscard.size() > 0) {
    auto discard = immediateDiscard.back();
    immediateDiscard.pop_back();
    return discard;
  }

  pieceSet discardOptions;

  // Document pieces in play, if anything adds up to 4 (nothing left), discard.
  for (pieceSet::iterator set = possibleTriples.begin(); set != possibleTriples.end(); set++) {
    auto piece = set->first;
    pieceSet::iterator setContainsPiece = allDiscards.find(piece);
    if(setContainsPiece != allDiscards.end()) {
      if (set->second + setContainsPiece->second == 4 && set->second < 3) {
        DecrementPiece(Mahjong::Piece(piece), possibleTriples);
        return Mahjong::Piece(piece);
      } else if (set->second > 0) {
        IncrementPiece(Mahjong::Piece(piece), discardOptions);
      }
    }
  }


  auto minValue = *std::min_element(discardOptions.begin(), discardOptions.end(), [](const auto& l, const auto& r) { return l.second < r.second; });
  auto discardPiece = Mahjong::Piece(minValue.first);
  DecrementPiece(discardPiece, possibleTriples);
  return discardPiece;
}

auto FastTanyao::ReceiveEvent(Mahjong::Event e) -> void {
  const Mahjong::Piece eventPiece = Mahjong::Piece(e.piece);
  if (e.type < Mahjong::Event::Decline && e.decision) {
    if (e.type < decidedDecision.type) {
      decidedDecision = e;
    }
    return;
  }

  switch(e.type) {
    case Mahjong::Event::Dora:
      if (ShouldKeep(eventPiece)) {
          validDoras.push_back(eventPiece);
        }
      break;
    case Mahjong::Event::Discard:
      if (e.decision) {
        ProcessNewPiece(eventPiece);
      } else {
        RecordDiscard(eventPiece);
      }
      break;
  }
}

auto FastTanyao::RetrieveDecision() -> Mahjong::Event{
  if (decidedDecision.type == Mahjong::Event::Discard) {
    decidedDecision.piece = ChooseDiscard().raw_value();
  }

  auto finalEvent = decidedDecision;
  decidedDecision.type = Mahjong::Event::End;
  return finalEvent;
}
