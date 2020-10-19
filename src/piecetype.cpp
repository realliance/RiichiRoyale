#include <string>
#include "piecetype.h"

auto Piece::toUint8_t() const -> uint8_t {
  return p & ~uint8_t(RED_FIVE);
}

auto Piece::raw_value() const -> uint8_t {
  return p;
}

auto Piece::isHonor() const -> bool{
  return (p & CHARACTER_SUIT) == HONOR_SUIT;
}

auto Piece::isTerminal() const -> bool{
  return (p & TERMINAL_BIT) != ERROR_PIECE;
}

auto Piece::getSuit() const -> uint8_t {
  return p & 3<<5;
}

auto Piece::getPieceNum() const -> uint8_t {
  return p & 15;
}

auto Piece::isGreen() const -> bool {
  if(p == GREEN_DRAGON){
    return true;
  }
  if(getSuit() != BAMBOO_SUIT){
    return false;
  }
  if(((getPieceNum() % 2) == 1) && getPieceNum() != 3){
    return false;
  }
  return true;
}

auto Piece::fromWind(Wind w) -> Piece {
  switch (w){
    case East:
      return Piece(EAST_WIND);
    case South:
      return Piece(SOUTH_WIND);
    case West:
      return Piece(WEST_WIND);
    case North:
      return Piece(NORTH_WIND);
    default:
      return Piece(ERROR_PIECE);
  }
}

auto Piece::toStr() const -> std::string{
  if(STR_MAP.contains(p)){
    return STR_MAP.at(p);
  }
  return STR_MAP.at(0); //ERRORPIECE
}

const std::map<uint8_t,std::string> Piece::STR_MAP = {
    {ERROR_PIECE, "ERRORPIECE"},
    {RED_DRAGON , "Red Dragon"},
    {WHITE_DRAGON , "White Dragon"},
    {GREEN_DRAGON , "Green Dragon"},
    {ONE_CHARACTER , "Man1"},
    {TWO_CHARACTER , "Man2"},
    {THREE_CHARACTER , "Man3"},
    {FOUR_CHARACTER , "Man4"},
    {RED_FIVE_CHARACTER , "Man5-Dora"},
    {FIVE_CHARACTER , "Man5"},
    {SIX_CHARACTER , "Man6"},
    {SEVEN_CHARACTER , "Man7"},
    {EIGHT_CHARACTER , "Man8"},
    {NINE_CHARACTER , "Man9"},
    {SOUTH_WIND , "South"},
    {NORTH_WIND , "North"},
    {ONE_PIN , "Pin1"},
    {TWO_PIN , "Pin2"},
    {THREE_PIN , "Pin3"},
    {FOUR_PIN , "Pin4"},
    {RED_FIVE_PIN , "Pin5-Dora"},
    {FIVE_PIN , "Pin5"},
    {SIX_PIN , "Pin6"},
    {SEVEN_PIN , "Pin7"},
    {EIGHT_PIN , "Pin8"},
    {NINE_PIN , "Pin9"},
    {WEST_WIND , "West"},
    {ONE_BAMBOO , "Sou1"},
    {TWO_BAMBOO , "Sou2"},
    {THREE_BAMBOO , "Sou3"},
    {FOUR_BAMBOO , "Sou4"},
    {RED_FIVE_BAMBOO , "Sou5-Dora"},
    {FIVE_BAMBOO , "Sou5"},
    {SIX_BAMBOO , "Sou6"},
    {SEVEN_BAMBOO , "Sou7"},
    {EIGHT_BAMBOO , "Sou8"},
    {NINE_BAMBOO , "Sou9"},
    {EAST_WIND , "East"},
};
