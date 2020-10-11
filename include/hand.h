#pragma once
#include "pieces.h"
#include "enum.h"

//concealed kan, kan, pon, chi
using CallType = EventType;

struct Meld{
  CallType Meld;
  std::vector<Piece> Pieces;
};

struct Hand{
  std::vector<Meld> melds;
  std::vector<Piece> discards;
  std::vector<Piece> live;
  bool open = false;
  bool riichi = false;
};