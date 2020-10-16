#include "hand.h"
#include <ostream>  // for operator<<, basic_ostream, endl, basic_ostream::o...
#include <string>   // for operator<<, char_traits

auto Hand::begin() const -> const_iterator{
  return const_iterator(this, false);
}

auto Hand::end() const -> const_iterator{
  return const_iterator(this, true);
}


auto Hand::const_iterator::operator++() -> const_iterator&{
  if( ( !hand->melds.empty() && position == meldPosition->pieces.end()) ||
    (position == hand->live.end() && hand->melds.empty()))
  {
    return *this;
  }
  ++position;

  //Iterator is valid
  if(position != hand->live.end() || position != meldPosition->pieces.end()){
    return *this;
  }

  //Iterator is at end;
  if(hand->melds.empty()){
    return *this;
  }

  //Set Iterator to the start of the melds
  if(!inMelds){
    position = meldPosition->pieces.begin();
    inMelds = true;
    return *this;
  }

  //Check to see if iterator is at the end of the melds
  if((meldPosition+1) == hand->melds.end()){
    return *this;
  }

  //Move Forward a meld
  meldPosition++;
  position = meldPosition->pieces.begin();
  return *this;
}

auto Hand::const_iterator::operator++(int) -> const_iterator{
  const_iterator itr(*this);
  operator++();
  return itr;
}

auto Hand::const_iterator::operator*() const -> Piece{
  if(inMelds && position != meldPosition->pieces.end()){
    return *position;
  }
  if(position != hand->live.end()){
    return *position;
  }
  return ERROR_PIECE;
}

auto Hand::const_iterator::operator!=(const Hand::const_iterator& other) const -> bool {
  return position != other.position;
}

auto operator<<(std::ostream& os, const Hand& hand) -> std::ostream&{
  os << "{ open: " << hand.open; 
  os << " riichi: " << hand.riichi;
  os << " riichiPieceDiscard: " << hand.riichiPieceDiscard;
  os << " riichiRound: " << hand.riichiRound;
  os << "live: [" << std::endl;;
  for(const auto & piece : hand.live){
    os << piece.toStr() << ", ";
  }
  os << "]" << std::endl;
  os << "melds: [" << std::endl;
  for(const auto & meld : hand.melds){
    os << meld << ", ";
  }
  os << "]" << std::endl;
  os << "discards: [" << std::endl;
  for(const auto & piece : hand.discards){
    os << piece.toStr() << ", ";
  }
  os << "]" << std::endl;
  os << "}";
  return os;
}
