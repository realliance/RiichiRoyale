#pragma once
#include <iosfwd>       // for ostream
#include <string>       // for allocator, string
#include "piecetype.h"  // for Piece

//concealed kan, kan, pon, chi
enum MeldType {
  ChiMeld,
  KanMeld,
  PonMeld,
  ConcealedKanMeld
};

inline auto MeldTypeToStr(MeldType s) -> std::string{
  switch(s){
    case ChiMeld:
      return "ChiMeld";
    case KanMeld:
      return "KanMeld";
    case PonMeld:
      return "PonMeld";
    case ConcealedKanMeld:
      return "ConcealedKanMeld";
    default:
      return "InvalidMeldType";

  }
}

struct Meld{
  MeldType type;
  Piece start;
  auto inline operator==(Meld other)const -> bool {
    if(type != other.type){
      return false;
    }
    return start == other.start;
  }
};

auto operator<<(std::ostream& os, const Meld& meld) -> std::ostream&;
