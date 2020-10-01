import os
import pygame
from pygame.surface import Surface
from riichiroyale.game import Tile
from riichiroyale.utils import load_image

SMALL_TILE_SIZE = [30,40]
TILE_SIZE = [51,68]

SUIT_PREFIX = {
  Tile.BAMBOO_SUIT: 'Sou',
  Tile.CHARACTER_SUIT: 'Man',
  Tile.PIN_SUIT: 'Pin'
}

HONOR_NAME = {
  Tile.EAST_WIND: 'Ton',
  Tile.WEST_WIND: 'Shaa',
  Tile.NORTH_WIND: 'Pei',
  Tile.SOUTH_WIND: 'Nan',
  Tile.RED_DRAGON: 'Chun',
  Tile.GREEN_DRAGON: 'Hatsu',
  Tile.WHITE_DRAGON: 'Haku'
}

def get_tile_front_name(tile):
  if tile == Tile.ERROR_PIECE:
    return 'Back'
  suit = Tile.getSuit(tile)
  if suit == Tile.HONOR_SUIT:
    return HONOR_NAME[tile]
  suffix = '-Dora' if Tile.isRedFive(tile) else ''
  return SUIT_PREFIX[suit] + str(Tile.getPieceNum(tile)) + suffix

def get_tile_front_path(dir_path, tile, file_suffix='svg'):
  return os.path.join(dir_path, get_tile_front_name(tile) + '.' + file_suffix)

def get_all_tile_front_paths(dir_path, file_suffix='svg'):
  full_list = list(map(int, Tile))
  full_path_dict = dict()
  for tile_id in full_list:
    tile = Tile(tile_id)
    if Tile.isBoardPiece(tile):
      full_path_dict[tile] = get_tile_front_path(dir_path, tile, file_suffix)
  full_path_dict[Tile.ERROR_PIECE] = get_tile_front_path(dir_path, Tile.ERROR_PIECE, file_suffix)
  return full_path_dict

def build_tile_surface_dict(dir_path, size, file_suffix='svg'):
  front_tile_paths = get_all_tile_front_paths(dir_path, file_suffix)
  back_path = os.path.join(dir_path, 'Front.' + file_suffix)
  back_base = Surface(size, pygame.SRCALPHA)
  load_image(back_path, back_base, size)
  result_dict = dict()
  for key, value in front_tile_paths.items():
    surface = back_base.copy()
    load_image(value, surface, size)
    result_dict[key] = surface.convert_alpha()
  return result_dict