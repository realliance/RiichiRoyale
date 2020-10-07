#pragma once
#include "pieces.h"
#include "enum.h"

struct Meld{
  CallType Meld;
  std::vector<Piece> Pieces;
};


struct Hand{
  explicit Hand(std::vector<Piece> pieces, Wind seat):live(pieces),seat(seat),open(false){}
  std::vector<Piece> live;
  std::vector<Meld> melds;
  std::vector<Piece> discards;
  Wind seat;
  bool open;
};
