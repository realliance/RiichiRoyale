from functools import reduce
from pygame.sprite import Group
from pygame import Rect
from riichiroyale.game import Tile, CallDirection
from riichiroyale.sprites import SMALL_TILE_SIZE, TileRender

def render_meld_hand(board_render, meld_hand, seat=0):
  VERTICAL = seat % 2 != 0
  group = Group()
  calculated_meld_dimensions = list(map(lambda x: x.calculate_meld_sprite_dimensions(*SMALL_TILE_SIZE), meld_hand))
  if len(calculated_meld_dimensions) > 2:
    row_meld_dimensions = [calculated_meld_dimensions[:2], calculated_meld_dimensions[2:]]
  else:
    row_meld_dimensions = [calculated_meld_dimensions[:2], []]
  max_meld_height = reduce(lambda acc, x: max(acc, reduce(lambda acc2, y: max(acc2, y[1]), x, 0)), row_meld_dimensions, 0)
  max_row_width = reduce(lambda acc, x: max(acc, reduce(lambda acc2, y: acc2 + y[0], x, 0)), row_meld_dimensions, 0)

  rect = Rect(board_render.surface.get_rect())
  offset = 0
  SEAT_POS = [
    (rect.width - max_row_width - 65, rect.height - 25 - (max_meld_height * 2)),
    (rect.width - 100 - (max_meld_height * 2), max_row_width),
    (400 - max_row_width, 25),
    (25 + (max_meld_height * 2), max_row_width),
  ]

  xpos, ypos = SEAT_POS[seat]

  i = 0
  for meld in meld_hand:
    if not VERTICAL:
      group.add(render_meld(board_render, meld, xpos + offset, ypos, meld_rotation=seat))
    else:
      group.add(render_meld(board_render, meld, xpos, ypos + offset, meld_rotation=seat))
    meld_width, meld_height = calculated_meld_dimensions[i]
    max_meld_height = max(max_meld_height, meld_height)
    offset += meld_width + 20
    i += 1
    if i == 2:
      offset = 0
      if VERTICAL:
        xpos += max_meld_height + 10
      else:
        ypos += max_meld_height + 10
  return group

def _get_tile_offset(offset, is_vertical, is_inverted):
  VERT_INVERT_LUT = {
    True: {
      True: 0,
      False: offset
    },
    False: {
      True: 0,
      False: 0
    }
  }

  return VERT_INVERT_LUT[is_vertical][is_inverted]

def render_meld(board_render, meld, xpos, ypos, meld_rotation=0):
  VERTICAL = meld_rotation % 2 != 0
  INVERT_CALL_OFFSET = meld_rotation > 1
  NONROTATED_MELD_PIECE = meld_rotation
  ROTATED_MELD_PIECE = meld_rotation + 1
  group = Group()
  direction = meld.call_direction

  i = 0
  tile_length = len(meld.tiles)
  while i < tile_length:
    tile = meld.tiles[i]
    rotation_i = 2 - i if meld_rotation in (1, 2) else i
    rotation = ROTATED_MELD_PIECE if CallDirection.should_rotate_tile(rotation_i, meld) else NONROTATED_MELD_PIECE

    offset = SMALL_TILE_SIZE[1] - SMALL_TILE_SIZE[0] if rotation == ROTATED_MELD_PIECE else NONROTATED_MELD_PIECE
    tile_posx = xpos + _get_tile_offset(offset, VERTICAL, INVERT_CALL_OFFSET)
    tile_posy = ypos + _get_tile_offset(offset, not VERTICAL, INVERT_CALL_OFFSET)
    tile_pos = (tile_posx, tile_posy)

    rendered_tile = Tile.ERROR_PIECE if direction == CallDirection.Concealed and i in (0, 3) else tile
    sprite = TileRender(board_render.small_dictionary, rendered_tile, tile_pos, small_tile=True, rotation=rotation)
    group.add(sprite)
    if rotation == 1 and meld.converted_kan:
      offset -= (5 + SMALL_TILE_SIZE[0])
      tile_posx = xpos + (offset if VERTICAL else 0)
      tile_posy = ypos + (offset if not VERTICAL else 0)
      tile_pos = (tile_posx, tile_posy)
      sprite = TileRender(board_render.small_dictionary, rendered_tile, tile_pos, small_tile=True, rotation=rotation)
      group.add(sprite)
      i += 1
    next_tile_shift = SMALL_TILE_SIZE[1] if rotation == ROTATED_MELD_PIECE else SMALL_TILE_SIZE[0]
    if VERTICAL:
      ypos += next_tile_shift + 5
    else:
      xpos += next_tile_shift + 5
    i += 1
  return group