from functools import reduce
from pygame.sprite import Group
from pygame import Rect
from riichiroyale.game import Tile, CallDirection
from riichiroyale.sprites import SMALL_TILE_SIZE, TileRender

def render_meld_hand(board_render, meld_hand, _player_tiles):
  group = Group()
  calculated_meld_dimensions = list(map(lambda x: x.calculate_meld_sprite_dimensions(*SMALL_TILE_SIZE), meld_hand))
  if len(calculated_meld_dimensions) > 2:
    row_meld_dimensions = [calculated_meld_dimensions[:2], calculated_meld_dimensions[2:]]
  else:
    row_meld_dimensions = [calculated_meld_dimensions[:2], []]
  max_meld_height = reduce(lambda acc, x: max(acc, reduce(lambda acc2, y: max(acc2, y[1]), x, 0)), row_meld_dimensions, 0)
  max_row_width = reduce(lambda acc, x: max(acc, reduce(lambda acc2, y: acc2 + y[0], x, 0)), row_meld_dimensions, 0)

  rect = Rect(board_render.surface.get_rect())
  xpos = rect.width - max_row_width - 25
  ypos = rect.height - 5
  xpos_offset = 0
  ypos -= (max_meld_height * 2)
  i = 0
  for meld in meld_hand:
    group.add(render_meld(board_render, meld, xpos + xpos_offset, ypos))
    meld_width, meld_height = calculated_meld_dimensions[i]
    max_meld_height = max(max_meld_height, meld_height)
    xpos_offset = meld_width + 20
    i += 1
    if i == 2:
      xpos_offset = 0
      ypos += max_meld_height + 10
  return group

def render_meld(board_render, meld, xpos, ypos):
  group = Group()
  direction = meld.call_direction

  i = 0
  tile_length = len(meld.tiles)
  while i < tile_length:
    tile = meld.tiles[i]
    rotation = 1 if CallDirection.should_rotate_tile(i, meld) else 0
    y_offset = SMALL_TILE_SIZE[1] - SMALL_TILE_SIZE[0] if rotation == 1 else 0
    tile_pos = (xpos, ypos + y_offset)
    rendered_tile = Tile.ERROR_PIECE if direction == CallDirection.Concealed and i in (0, 3) else tile
    sprite = TileRender(board_render.small_dictionary, rendered_tile, tile_pos, small_tile=True, rotation=rotation)
    group.add(sprite)
    if rotation == 1 and meld.converted_kan:
      tile_pos = (xpos, ypos + y_offset - (5 + SMALL_TILE_SIZE[0]))
      sprite = TileRender(board_render.small_dictionary, rendered_tile, tile_pos, small_tile=True, rotation=rotation)
      group.add(sprite)
      i += 1
    xpos += SMALL_TILE_SIZE[1] if rotation == 1 else SMALL_TILE_SIZE[0]
    xpos += 5
    i += 1
  return group