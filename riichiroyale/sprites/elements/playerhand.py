from pygame.sprite import Group
from pygame import Rect
from riichiroyale.sprites import TILE_SIZE, TileRender

def render_hand(board_render, player_pov):
  group = Group()
  board = board_render.board
  player = board.players[player_pov]

  rect = Rect(board_render.surface.get_rect())
  rect.bottom = TILE_SIZE[1] + 15

  tile_offset = 10
  full_hand_width = len(player.hand) * (TILE_SIZE[0] + tile_offset) - tile_offset
  xpos = (rect.width - full_hand_width) / 2
  tile_index = 0
  number_of_melded_tiles = len(player.melded_hand) * 3
  for tile in player.hand:
    if tile_index + number_of_melded_tiles == 13:
      xpos += 3 * tile_offset
    tile_sprite = TileRender(board_render.dictionary, tile, (xpos, -rect.y), owner=player, known_index=tile_index, interact=True)
    group.add(tile_sprite)
    xpos += TILE_SIZE[0] + tile_offset
    tile_index += 1
  return group