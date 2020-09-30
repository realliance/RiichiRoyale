from enum import Enum

class Tile(Enum):
    TERMINAL_BIT = 1<<7
    HONOR_SUIT = 0<<5
    BAMBOO_SUIT = 1<<5
    PIN_SUIT = 2<<5
    CHARACTER_SUIT = 3<<5
    RED_FIVE = 1<<4
    ERROR_PIECE = 0
    ONE_BAMBOO   = BAMBOO_SUIT | 1 | TERMINAL_BIT
    TWO_BAMBOO   = BAMBOO_SUIT | 2
    THREE_BAMBOO = BAMBOO_SUIT | 3
    FOUR_BAMBOO  = BAMBOO_SUIT | 4
    FIVE_BAMBOO  = BAMBOO_SUIT | 5
    RED_FIVE_BAMBOO = BAMBOO_SUIT | 5 | RED_FIVE
    SIX_BAMBOO   = BAMBOO_SUIT | 6
    SEVEN_BAMBOO = BAMBOO_SUIT | 7
    EIGHT_BAMBOO = BAMBOO_SUIT | 8
    NINE_BAMBOO  = BAMBOO_SUIT | 9 | TERMINAL_BIT
    ONE_PIN   = PIN_SUIT | 1 | TERMINAL_BIT
    TWO_PIN   = PIN_SUIT | 2
    THREE_PIN = PIN_SUIT | 3
    FOUR_PIN  = PIN_SUIT | 4
    FIVE_PIN  = PIN_SUIT | 5
    RED_FIVE_PIN = PIN_SUIT | 5 | RED_FIVE
    SIX_PIN   = PIN_SUIT | 6
    SEVEN_PIN = PIN_SUIT | 7
    EIGHT_PIN = PIN_SUIT | 8
    NINE_PIN  = PIN_SUIT | 9 | TERMINAL_BIT
    ONE_CHARACTER   = CHARACTER_SUIT | 1 | TERMINAL_BIT
    TWO_CHARACTER   = CHARACTER_SUIT | 2
    THREE_CHARACTER = CHARACTER_SUIT | 3
    FOUR_CHARACTER  = CHARACTER_SUIT | 4
    FIVE_CHARACTER  = CHARACTER_SUIT | 5
    RED_FIVE_CHARACTER = CHARACTER_SUIT | 5 | RED_FIVE
    SIX_CHARACTER   = CHARACTER_SUIT | 6
    SEVEN_CHARACTER = CHARACTER_SUIT | 7
    EIGHT_CHARACTER = CHARACTER_SUIT | 8
    NINE_CHARACTER  = CHARACTER_SUIT | 9 | TERMINAL_BIT
    EAST_WIND    = HONOR_SUIT | 1
    SOUTH_WIND   = HONOR_SUIT | 2
    WEST_WIND    = HONOR_SUIT | 3
    NORTH_WIND   = HONOR_SUIT | 4
    RED_DRAGON   = HONOR_SUIT | 5
    WHITE_DRAGON = HONOR_SUIT | 6
    GREEN_DRAGON = HONOR_SUIT | 7

  @staticmethod
  def isHonor(piece):
    return (piece.value & c_Piece.CHARACTER_SUIT.value) == c_Piece.ERROR_PIECE.value

  @staticmethod
  def isTerminal(piece):
    return (piece.value & c_Piece.TERMINAL_BIT.value) != c_Piece.ERROR_PIECE.value

  @staticmethod
  def getSuit(piece):
    return piece.value & 3<<4

  @staticmethod
  def getPieceNum(piece):
    return piece.value & 15

  def __lt__(self, other):
    if (c_Piece.getSuit(self) != c_Piece.getSuit(other)):
      return c_Piece.getSuit(self) > c_Piece.getSuit(other)
    return c_Piece.getPieceNum(self) < c_Piece.getPieceNum(other)
