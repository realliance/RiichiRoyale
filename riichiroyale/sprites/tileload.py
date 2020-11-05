import os
import pygame
from pygame.surface import Surface
from libmahjong import Piece, PieceType
from riichiroyale.utils import load_image

SMALL_TILE_SIZE = [30, 40]
TILE_SIZE = [51, 68]

SUIT_PREFIX = {
    PieceType.BAMBOO_SUIT: "Sou",
    PieceType.CHARACTER_SUIT: "Man",
    PieceType.PIN_SUIT: "Pin",
}

HONOR_NAME = {
    PieceType.EAST_WIND: "Ton",
    PieceType.WEST_WIND: "Shaa",
    PieceType.NORTH_WIND: "Pei",
    PieceType.SOUTH_WIND: "Nan",
    PieceType.RED_DRAGON: "Chun",
    PieceType.GREEN_DRAGON: "Hatsu",
    PieceType.WHITE_DRAGON: "Haku",
}


def get_tile_front_name(tile):
    if tile.get_raw_value() == PieceType.ERROR:
        return "Back"
    suit = tile.get_suit()
    if suit == PieceType.HONOR_SUIT:
        return HONOR_NAME[tile.get_raw_value()]
    suffix = "-Dora" if tile.is_red_five() else ""
    return SUIT_PREFIX[suit] + str(tile.get_piece_num()) + suffix


def get_tile_front_path(dir_path, tile, file_suffix="svg"):
    return os.path.join(dir_path, get_tile_front_name(tile) + "." + file_suffix)


def get_all_tile_front_paths(dir_path, file_suffix="svg"):
    full_list = list(map(int, PieceType.__members__.values()))
    full_path_dict = dict()
    for tile_id in full_list:
        tile = Piece(tile_id)
        if tile.is_board_piece():
            full_path_dict[tile.get_raw_value()] = get_tile_front_path(dir_path, tile, file_suffix)
    full_path_dict[PieceType.ERROR] = get_tile_front_path(
        dir_path, Piece(PieceType.ERROR), file_suffix
    )
    return full_path_dict


def build_tile_surface_dict(dir_path, size, file_suffix="svg"):
    front_tile_paths = get_all_tile_front_paths(dir_path, file_suffix)
    back_path = os.path.join(dir_path, "Front." + file_suffix)
    back_base = Surface(size, pygame.SRCALPHA)
    load_image(back_path, back_base, size)
    result_dict = dict()
    for key, value in front_tile_paths.items():
        surface = back_base.copy()
        load_image(value, surface, size)
        result_dict[key] = surface.convert_alpha()
    return result_dict
