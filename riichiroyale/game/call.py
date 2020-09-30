from enum import Enum
from .tile import Tile

class Call(Enum):
  Pon = 0
  Chi = 1
  Kan = 2
  Ron = 3
  Concealed_Kan = 4
  Riichi = 5

tile_indices = get_indexes = lambda xs, x: [i for (y, i) in zip(xs, range(len(xs))) if x == y]

def concealed_kan_possible(hand):
  index = 0
  for tile in hand:
    tiles_found = tile_indices(hand[index:], tile)
    if len(tiles_found) == 4:
      return True
    index += 1
  return False

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