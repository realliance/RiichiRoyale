import random
import copy
from riichiroyale.game import Tile

PIECE_SET = [
  Tile.ONE_BAMBOO,
  Tile.TWO_BAMBOO,
  Tile.THREE_BAMBOO,
  Tile.FOUR_BAMBOO,
  Tile.SIX_BAMBOO,
  Tile.SEVEN_BAMBOO,
  Tile.EIGHT_BAMBOO,
  Tile.NINE_BAMBOO,
  Tile.ONE_PIN,
  Tile.TWO_PIN,
  Tile.THREE_PIN,
  Tile.FOUR_PIN,
  Tile.SIX_PIN,
  Tile.SEVEN_PIN,
  Tile.EIGHT_PIN,
  Tile.NINE_PIN,
  Tile.ONE_CHARACTER,
  Tile.TWO_CHARACTER,
  Tile.THREE_CHARACTER,
  Tile.FOUR_CHARACTER,
  Tile.SIX_CHARACTER,
  Tile.SEVEN_CHARACTER,
  Tile.EIGHT_CHARACTER,
  Tile.NINE_CHARACTER,
  Tile.NORTH_WIND,
  Tile.EAST_WIND,
  Tile.SOUTH_WIND,
  Tile.WEST_WIND,
  Tile.RED_DRAGON,
  Tile.GREEN_DRAGON,
  Tile.WHITE_DRAGON
] * 4 + [Tile.FIVE_BAMBOO, Tile.FIVE_CHARACTER, Tile.FIVE_PIN] * 3 + [Tile.RED_FIVE_BAMBOO, Tile.RED_FIVE_CHARACTER, Tile.RED_FIVE_PIN]

def generate_start_board():
  board = copy.deepcopy(PIECE_SET)
  random.shuffle(board)
  return board[14:], board[:14]

