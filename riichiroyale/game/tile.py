from enum import IntEnum

class Tile(IntEnum):
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
    return (piece.value & Tile.CHARACTER_SUIT) == Tile.ERROR_PIECE

  @staticmethod
  def isTerminal(piece):
    return (piece.value & Tile.TERMINAL_BIT) != Tile.ERROR_PIECE

  @staticmethod
  def isBoardPiece(piece):
    return piece & 0x0F != 0

  @staticmethod
  def getSuit(piece):
    return Tile(piece.value & 3<<5)

  @staticmethod
  def isRedFive(piece):
    return piece & Tile.RED_FIVE != Tile.ERROR_PIECE

  @staticmethod
  def getPieceNum(piece):
    return piece.value & 15

  def __lt__(self, other):
    if (Tile.getSuit(self) != Tile.getSuit(other)):
      return Tile.getSuit(self) > Tile.getSuit(other)
    return Tile.getPieceNum(self) < Tile.getPieceNum(other)

  def __add__(self, other):
    if (Tile.isHonor(self)):
      raise "Cannot add with Honor Tiles"
    new_number = Tile.getPieceNum(self) + other
    if new_number > 9:
      raise "Integer overflow on tile addition"
    suit = Tile.getSuit(self)
    is_terminal = new_number in (1, 9)
    if is_terminal:
      return Tile(suit | new_number | Tile.TERMINAL_BIT)
    return Tile(suit | new_number)

  def __sub__(self, other):
    if (Tile.isHonor(self)):
      raise "Cannot subtract with Honor Tiles"
    new_number = Tile.getPieceNum(self) - other
    if new_number < 1:
      raise "Integer underflow on tile subtraction"
    suit = Tile.getSuit(self)
    is_terminal = new_number in (1, 9)
    if is_terminal:
      return Tile(suit | new_number | Tile.TERMINAL_BIT)
    return Tile(suit | new_number)
