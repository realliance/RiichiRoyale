#pragma once
#include <ext/alloc_traits.h>  // for __alloc_traits<>::value_type
#include <iosfwd>              // for ostream
#include <string>              // for string
#include <vector>              // for vector, allocator, vector<>::const_ite...
#include "piecetype.h"         // for Piece

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
  std::vector<Piece> pieces;
  auto inline operator==(Meld other)const -> bool {
    if(type != other.type){
      return false;
    }
    if(type == ChiMeld){
      if(pieces[0] == other.pieces[0]){
        if(pieces[1] == other.pieces[1]){
          return pieces[2] == other.pieces[2];
        }else if(pieces[1] == other.pieces[2]){
          return pieces[2] == other.pieces[1];
        }
      }else if(pieces[0] == other.pieces[1]){
        if(pieces[1] == other.pieces[0]){
          return pieces[2] == other.pieces[2];
        }else if(pieces[1] == other.pieces[2]){
          return pieces[2] == other.pieces[0];
        }
      } else if(pieces[0] == other.pieces[2]){
        if(pieces[1] == other.pieces[0]){
          return pieces[2] == other.pieces[1];
        }else if(pieces[1] == other.pieces[1]){
          return pieces[2] == other.pieces[0];
        }
      }
      return false;
    }
    return pieces[0] == other.pieces[0];
  }
  auto inline begin() -> std::vector<Piece>::iterator{ return pieces.begin(); }
  auto inline end() -> std::vector<Piece>::iterator{ return pieces.end(); }
  auto inline begin() const -> std::vector<Piece>::const_iterator{ return pieces.begin(); }
  auto inline end() const -> std::vector<Piece>::const_iterator{ return pieces.end(); }
};

auto operator<<(std::ostream& os, const Meld& meld) -> std::ostream&;
