from enum import Enum, IntEnum
from .tile import Tile

class Call(Enum):
  Pon = 0
  Chi = 1
  Kan = 2
  Ron = 3
  Concealed_Kan = 4
  Riichi = 5

class CallDirection(IntEnum):
  Left = 0
  Forward = 1
  Right = 2
  Concealed = 3

  @staticmethod
  def should_rotate_tile(i, meld):
    number_of_tiles = len(meld.tiles)
    direction = meld.call_direction
    converted_pon_to_kan = meld.converted_kan
    if direction == CallDirection.Left:
      return i in ([0,1] if converted_pon_to_kan else [0])
    if direction == CallDirection.Forward:
      return i in ([1,2] if converted_pon_to_kan else [1])
    if direction == CallDirection.Right:
      return i in ([number_of_tiles - 1,number_of_tiles - 2] if converted_pon_to_kan else [number_of_tiles - 1])
    return False
    

tile_indices = get_indexes = lambda xs, x: [i for (y, i) in zip(xs, range(len(xs))) if x == y]

def concealed_kan_possible(hand):
  index = 0
  for tile in hand:
    tiles_found = tile_indices(hand[index:], tile)
    if len(tiles_found) == 4:
      return True
    index += 1
  return False

def is_kan(tiles):
  if len(tiles) != 4:
    return False
  return kan_possible(tiles[1:], tiles[0])

def is_pon(tiles):
  if len(tiles) >= 3:
    return False
  return pon_possible(tiles[1:], tiles[0])

def is_chi(tiles):
  if len(tiles) != 3:
    return False
  return chi_possible(tiles[1:], tiles[0])

def kan_possible(hand, tile):
  return len(tile_indices(hand, tile)) >= 3

def pon_possible(hand, tile):
  return len(tile_indices(hand, tile)) >= 2

def chi_possible(hand, tile):
  if Tile.isHonor(tile):
    return False

  if Tile.getPieceNum(tile) <= 7:
    if tile + 1 in hand and tile + 2 in hand:
      return True

  if not Tile.isTerminal(tile):
    if tile - 1 in hand and tile + 1 in hand:
      return True

  if Tile.getPieceNum(tile) >= 3:
    if tile - 2 in hand and tile - 1 in hand:
      return True

  return False