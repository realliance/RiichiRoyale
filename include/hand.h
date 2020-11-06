#pragma once
#include <algorithm>    // for sort
#include <iosfwd>       // for ostream, ptrdiff_t
#include <vector>       // for vector<>::iterator, vector, vector<>::const_i...
#include "meld.h"       // for Meld
#include "piecetype.h"  // for Piece

namespace Mahjong {

  class Hand{
  public:
    Hand() = default;
    explicit Hand(std::vector<Piece> live): live(live){}
    auto inline sort() -> void { 
      std::sort(live.begin(),live.end());
    }
    std::vector<Piece> live;
    std::vector<Meld> melds;
    std::vector<Piece> discards;
    bool open = false;
    bool riichi = false;
    int riichiPieceDiscard = -1;
    int riichiRound = -1;
  };


}

auto operator<<(std::ostream& os, const Mahjong::Hand& hand) -> std::ostream&;
