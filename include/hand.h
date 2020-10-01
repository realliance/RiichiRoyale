#pragma once
#include "pieces.h"

enum Melds{
  Chi,
  Pon,
  Kan,
  ConcealedKan
};

struct Meld{
  Melds Meld;
  std::vector<Piece> Pieces;
};

enum Winds{
  East,
  South,
  West,
  North
};

struct Hand{
  explicit Hand(std::vector<Piece> pieces,Winds seat):live(pieces),seat(seat),open(false){}
  std::vector<Piece> live;
  std::vector<Meld> melds;
  std::vector<Piece> discards;
  Winds seat;
  bool open;
};
