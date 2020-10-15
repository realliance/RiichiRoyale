#pragma once
#include <vector>

#include "piecetype.h"
#include "enum.h"
#include "hands.h"

//concealed kan, kan, pon, chi
using CallType = EventType;

struct Meld{
  CallType type;
  std::vector<Piece> pieces;
  auto inline operator==(Meld other)const -> bool {
    if(type != other.type){
      return false;
    }
    if(type == Chi){
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
};

class Hand{
public:
  std::vector<Meld> melds;
  std::vector<Piece> discards;
  std::vector<Piece> live;
  bool open = false;
  bool riichi = false;
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
