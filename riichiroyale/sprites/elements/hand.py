from pygame.sprite import Group
from pygame import Rect
from riichiroyale.game import Tile
from riichiroyale.sprites import SMALL_TILE_SIZE, TILE_SIZE, TileRender


def render_hand(board_render, player_pov):
  player = board_render.board.players[player_pov]

  rect = Rect(board_render.surface.get_rect())
  rect.y = rect.height - (TILE_SIZE[1] + 15)
  tile_offset = 10
  full_hand_width = len(player.hand) * (TILE_SIZE[0] + tile_offset) - tile_offset
  xpos = (rect.width - full_hand_width) / 2
  return _render_hand(board_render, TILE_SIZE, player, player.hand, player.melded_hand, rect, xpos, tile_offset)


def render_hidden_hand(board_render, pov, seat):
  player = board_render.board.players[pov]
  rect = Rect(board_render.surface.get_rect())
  tile_offset = 5
  full_hand_width = len(player.hand) * (SMALL_TILE_SIZE[0] + tile_offset) - tile_offset

  TILE_POS = [
    ((rect.width - full_hand_width) / 2, (SMALL_TILE_SIZE[1] + 15)),
    ((SMALL_TILE_SIZE[1] + 15), (rect.height - full_hand_width) / 2),
    ((rect.width - full_hand_width) / 2, 10),
    (10, (rect.height - full_hand_width) / 2)
  ]

  xpos, ypos = TILE_POS[seat]
  if seat == 0:
    rect.bottom = ypos
  elif seat == 1:
    rect.x = rect.width - xpos
  elif seat == 2:
    rect.y = ypos
  elif seat == 3:
    rect.x = xpos

  hand = [Tile.ERROR_PIECE] * len(player.hand)

  if seat % 2 == 0:
    return _render_hand(board_render, SMALL_TILE_SIZE, player, hand, player.melded_hand, rect, xpos, tile_offset=tile_offset, should_interact=False, small_tile=True)
  return _render_vertical_hand(board_render, SMALL_TILE_SIZE, player, hand, player.melded_hand, rect, ypos, tile_offset=tile_offset, should_interact=False, rotation=1, small_tile=True)

def _render_hand(board_render, tile_dict, player, hand, melded_hand, rect, xpos, tile_offset=10, should_interact=True, small_tile=False):
  group = Group()

  tile_index = 0
  number_of_melded_tiles = len(melded_hand) * 3
  for tile in hand:
    if tile_index + number_of_melded_tiles == 13:
      xpos += 3 * tile_offset
    tile_sprite = TileRender(board_render.small_dictionary if small_tile else board_render.dictionary, tile, (xpos, rect.y), owner=player, known_index=tile_index, interact=should_interact, small_tile=small_tile)
    group.add(tile_sprite)
    xpos += tile_dict[0] + tile_offset
    tile_index += 1
  return group

def _render_vertical_hand(board_render, tile_dict, player, hand, melded_hand, rect, ypos, tile_offset=10, rotation=0, should_interact=True, small_tile=False):
  group = Group()

  tile_index = 0
  number_of_melded_tiles = len(melded_hand) * 3
  for tile in hand:
    if tile_index + number_of_melded_tiles == 13:
      ypos += 3 * tile_offset
    tile_sprite = TileRender(board_render.small_dictionary if small_tile else board_render.dictionary, tile, (rect.x, ypos), owner=player, known_index=tile_index, interact=should_interact, rotation=rotation, small_tile=small_tile)
    group.add(tile_sprite)
    ypos += tile_dict[0] + tile_offset
    tile_index += 1
  return group