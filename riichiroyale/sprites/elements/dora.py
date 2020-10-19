from pygame.sprite import Group
from pygame import Rect
from riichiroyale.game import Tile
from riichiroyale.sprites import SMALL_TILE_SIZE, TileRender

def render_dora_pile(board_render):
  group = Group()
  board = board_render.board
  dora_revealed = board.get_revealed_dora()
  dora_revealed += [Tile.ERROR_PIECE] * (5 - board.dora_revealed)

  tile_offset = 10
  full_width = (SMALL_TILE_SIZE[0] + tile_offset) * 5 - tile_offset
  xpos = 0

  rect = Rect(board_render.surface.get_rect())
  rect.y = 10
  rect.x = rect.width - full_width - 10
  for tile in dora_revealed:
    tile_pos = (rect.x + xpos, rect.y)
    tile_sprite = TileRender(board_render.small_dictionary, tile, tile_pos, small_tile=True)
    group.add(tile_sprite)
    xpos += tile_offset + SMALL_TILE_SIZE[0]
  return group