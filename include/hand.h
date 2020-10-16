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
    for(auto & meld : melds){
      std::sort(meld.begin(),meld.end());
    } 
  }
  std::vector<Piece> live;
  std::vector<Meld> melds;
  std::vector<Piece> discards;
  bool open = false;
  bool riichi = false;
  bool riichiPieceDiscard = false;
  int riichiRound = -1;
  class const_iterator {
    const Hand* hand;
    std::vector<Meld>::const_iterator meldPosition;
    std::vector<Piece>::const_iterator position;
    bool inMelds;
    friend Hand;
    explicit const_iterator(const Hand* hand, bool end):
      hand(hand),meldPosition(hand->melds.begin()),
      position(hand->live.begin()),inMelds(false)
    {
        if(!end){
          return;
        }
        if(hand->melds.empty()){
          position = hand->live.end();
          return;
        }
        meldPosition = --hand->melds.end();
        inMelds = true;
        position = hand->melds.back().pieces.end();
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
