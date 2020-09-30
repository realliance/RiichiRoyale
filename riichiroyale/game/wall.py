import random
import copy
from riichiroyale.game import Tile

PIECE_SET = [
  Tile.ONE_BAMBOO,
  Tile.TWO_BAMBOO
] * 4

def generate_start_board():
  board = copy.deepcopy(PIECE_SET)
  random.shuffle(board)
  return board

