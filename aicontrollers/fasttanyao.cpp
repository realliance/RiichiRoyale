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
  return !piece.isHonor() && !piece.isTerminal();
}

auto FastTanyao::OutputSet(uint8_t id, pieceSet& set) -> void {
  std::cout << "(" << Mahjong::Piece(id).toStr() << ", " << unsigned(set.at(id)) << ")" << std::endl;
}

auto FastTanyao::IncrementPiece(Mahjong::Piece piece, pieceSet& set) -> void {
  IncreasePiece(piece, set, 1);
}

auto FastTanyao::IncreasePiece(Mahjong::Piece piece, pieceSet& set, uint8_t count) -> void {
  pieceSet::iterator setContainsPiece = set.find(piece.raw_value());
  if(setContainsPiece != set.end()) {
    setContainsPiece->second++;
    OutputSet(piece.raw_value(), set);
    return;
  }

  set.insert(std::pair<uint8_t, uint8_t>(piece.raw_value(), 1));
  OutputSet(piece.raw_value(), set);
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
    std::cout << Mahjong::Piece(piece).toStr() << " should be immediately discarded." << std::endl;
    return;
  }

  IncrementPiece(piece, possibleTriples);
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
    std::cout << "Chose piece " << discard.toStr() << " to discard (imm discard)" << std::endl;
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
        std::cout << "Chose piece " << Mahjong::Piece(piece).toStr() << " to discard (all pieces in play)" << std::endl;
        return Mahjong::Piece(piece);
      } else if (set->second > 0) {
        IncreasePiece(Mahjong::Piece(piece), discardOptions, set->second);
      }
    }
  }


  auto minValue = *std::min_element(discardOptions.begin(), discardOptions.end(), [](const auto& l, const auto& r) { return l.second < r.second; });
  std::cout << unsigned(minValue.first) << ", " << unsigned(minValue.second) << std::endl;
  auto discardPiece = Mahjong::Piece(minValue.first);
  DecrementPiece(discardPiece, possibleTriples);
  std::cout << "Chose piece " << discardPiece.toStr() << " to discard (min risk)" << std::endl;
  return discardPiece;
}

auto FastTanyao::ReceiveEvent(Mahjong::Event e) -> void {
  const Mahjong::Piece eventPiece = Mahjong::Piece(e.piece);
  if (e.type <= Mahjong::Event::Discard && e.decision) {
    if (e.type == Mahjong::Event::Discard || ShouldKeep(e.piece)) {
      if (e.type < decidedDecision.type) {
        std::cout << "Choosing decision " << e.type << std::endl;
        decidedDecision = e;
      }
    }
  }

  switch(e.type) {
    case Mahjong::Event::Dora:
      if (ShouldKeep(eventPiece)) {
          validDoras.push_back(eventPiece);
        }
      break;
    case Mahjong::Event::Pon:
      IncreasePiece(eventPiece, allDiscards, 3);
      break;
    case Mahjong::Event::Kan:
      IncreasePiece(eventPiece, allDiscards, 4);
    case Mahjong::Event::Chi:
      IncrementPiece(eventPiece, allDiscards);
      IncrementPiece(eventPiece + 1, allDiscards);
      IncrementPiece(eventPiece + 2, allDiscards);
    case Mahjong::Event::Discard:
      if (e.decision) {
        ProcessNewPiece(eventPiece);
      } else {
        IncrementPiece(eventPiece, allDiscards);
      }
      break;
  }
}

auto FastTanyao::RetrieveDecision() -> Mahjong::Event{
  if (decidedDecision.type == Mahjong::Event::Discard) {
    decidedDecision.piece = ChooseDiscard().raw_value();
  }

  auto finalEvent = decidedDecision;
  decidedDecision.type = Mahjong::Event::Discard;
  std::cout << "Sending Decision: " << finalEvent << std::endl;
  return finalEvent;
}
