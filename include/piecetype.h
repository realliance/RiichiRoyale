#pragma once
#include <stdint.h>
#include <map>
#include "enum.h"

// TERMINAL_BIT, SUIT_2, RED_FIVE, PIECE_4
// For honor pieces suit bits are set to 00.


enum PieceType {
    TERMINAL_BIT = 1<<7,
    HONOR_SUIT = 0<<5,
    BAMBOO_SUIT = 1<<5,
    PIN_SUIT = 2<<5,
    CHARACTER_SUIT = 3<<5,
    RED_FIVE = 1<<4,
    ERROR_PIECE = 0,
    ONE_BAMBOO   = BAMBOO_SUIT | 1 | TERMINAL_BIT,
    TWO_BAMBOO   = BAMBOO_SUIT | 2,
    THREE_BAMBOO = BAMBOO_SUIT | 3,
    FOUR_BAMBOO  = BAMBOO_SUIT | 4,
    FIVE_BAMBOO  = BAMBOO_SUIT | 5,
    RED_FIVE_BAMBOO = BAMBOO_SUIT | 5 | RED_FIVE,
    SIX_BAMBOO   = BAMBOO_SUIT | 6,
    SEVEN_BAMBOO = BAMBOO_SUIT | 7,
    EIGHT_BAMBOO = BAMBOO_SUIT | 8,
    NINE_BAMBOO  = BAMBOO_SUIT | 9 | TERMINAL_BIT,
    ONE_PIN   = PIN_SUIT | 1 | TERMINAL_BIT,
    TWO_PIN   = PIN_SUIT | 2,
    THREE_PIN = PIN_SUIT | 3,
    FOUR_PIN  = PIN_SUIT | 4,
    FIVE_PIN  = PIN_SUIT | 5,
    RED_FIVE_PIN = PIN_SUIT | 5 | RED_FIVE,
    SIX_PIN   = PIN_SUIT | 6,
    SEVEN_PIN = PIN_SUIT | 7,
    EIGHT_PIN = PIN_SUIT | 8,
    NINE_PIN  = PIN_SUIT | 9 | TERMINAL_BIT,
    ONE_CHARACTER   = CHARACTER_SUIT | 1 | TERMINAL_BIT,
    TWO_CHARACTER   = CHARACTER_SUIT | 2,
    THREE_CHARACTER = CHARACTER_SUIT | 3,
    FOUR_CHARACTER  = CHARACTER_SUIT | 4,
    FIVE_CHARACTER  = CHARACTER_SUIT | 5,
    RED_FIVE_CHARACTER = CHARACTER_SUIT | 5 | RED_FIVE,
    SIX_CHARACTER   = CHARACTER_SUIT | 6,
    SEVEN_CHARACTER = CHARACTER_SUIT | 7,
    EIGHT_CHARACTER = CHARACTER_SUIT | 8,
    NINE_CHARACTER  = CHARACTER_SUIT | 9 | TERMINAL_BIT,
    EAST_WIND    = HONOR_SUIT | 1,
    SOUTH_WIND   = HONOR_SUIT | 2,
    WEST_WIND    = HONOR_SUIT | 3,
    NORTH_WIND   = HONOR_SUIT | 4,
    RED_DRAGON   = HONOR_SUIT | 5,
    WHITE_DRAGON = HONOR_SUIT | 6,
    GREEN_DRAGON = HONOR_SUIT | 7
};

class Piece{
private:
  static const std::map<uint8_t,std::string> STR_MAP;
  uint8_t p;
public:
  Piece():p(ERROR_PIECE){}
  Piece(uint8_t p):p(p){}

  auto value() const -> uint8_t;

  auto raw_value() const -> uint8_t;

  auto isHonor() const -> bool;

  auto isTerminal() const -> bool;

  auto getSuit() const -> int;

  auto getPieceNum() const -> int;
  
  auto isGreen() const -> bool;

  static auto fromWind(Wind w) -> Piece;

  auto toStr() const -> std::string;

  auto operator++() -> Piece&{
    if(isHonor()){
      p = ERROR_PIECE;
      return *this;
    }
    if(getPieceNum() == 9){
      p = ERROR_PIECE;
      return *this;
    }
    p++;
    return *this;
  }

  inline auto operator++(int) -> Piece{
    Piece _p(*this);
    operator++();
    return _p;
  }

  inline auto operator==(Piece rhs) const -> bool{
    return (p | RED_FIVE) == (rhs.p | RED_FIVE);
  }

  inline auto operator-(int i) const -> Piece{
    if(isHonor() || (getPieceNum()-i < 1)){
      return Piece(ERROR_PIECE);
    }
    if(getPieceNum()-i == 1){
      return Piece(((p-i) & ~uint8_t(RED_FIVE)) | TERMINAL_BIT);
    }
    return Piece(((p-i) & ~uint8_t(RED_FIVE)) & ~uint8_t(TERMINAL_BIT));
  }

  inline auto operator+(int i) const -> Piece{
    if(isHonor() || (getPieceNum()+i > 9)){
      return Piece(ERROR_PIECE);
    }
    if(getPieceNum()+i == 9){
      return Piece(((p+i) & ~uint8_t(RED_FIVE)) | TERMINAL_BIT);
    }
    return Piece(((p+i) & ~uint8_t(RED_FIVE)) & ~uint8_t(TERMINAL_BIT));
  }

  inline auto operator<(Piece other) -> bool{   
      if(getSuit() != other.getSuit()){
        return getSuit() > other.getSuit();
      }
      return getPieceNum() < other.getPieceNum();  
  };
};
