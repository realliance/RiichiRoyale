#pragma once
#include <algorithm>    // for sort
#include <iosfwd>       // for ostream, ptrdiff_t
#include <iterator>     // for forward_iterator_tag
#include <memory>       // for allocator_traits<>::value_type
#include <vector>       // for vector<>::iterator, vector, vector<>::const_i...
#include "meld.h"       // for Meld
#include "piecetype.h"  // for Piece

class Hand{
public:
  explicit Hand(std::vector<Piece> live): live(live){}
  auto inline sort() -> void { 
    std::sort(live.begin(),live.end());
  }
  std::vector<Piece> live;
  std::vector<Meld> melds;
  std::vector<Piece> discards;
  bool open = false;
  bool riichi = false;
  bool riichiPieceDiscard = false;
  int riichiRound = -1;
  class const_iterator {
    std::vector<Piece>::const_iterator itrBegin;
    std::vector<Piece>::const_iterator itrEnd;
    friend Hand;
    explicit const_iterator(const Hand* hand, bool end):itrBegin(hand->live.begin()),itrEnd(hand->live.end()) {
      if(end){
        itrBegin = itrEnd;
      }
    }
   public:
    auto operator++() -> const_iterator&;
    auto operator++(int) -> const_iterator;
    auto operator*() const -> Piece;
    auto operator!=(const const_iterator& other) const -> bool;
    // iterator traits
    using difference_type = std::ptrdiff_t;
    using pointer = const Piece*;
    using reference = const Piece&;
    using iterator_category = std::forward_iterator_tag;
  };
  auto begin() const -> const_iterator;
  auto end() const -> const_iterator;
};

auto operator<<(std::ostream& os, const Hand& hand) -> std::ostream&;
