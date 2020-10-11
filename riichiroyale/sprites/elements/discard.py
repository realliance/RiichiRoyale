from pygame.sprite import Group
from pygame import Rect
from riichiroyale.sprites import TILE_SIZE, SMALL_TILE_SIZE, TileRender

def render_discard_pile(board_render, player_id):
  group = Group()
  board = board_render.board
  player = board.players[0]

  if player.discard_pile is None:
    return group

  rect = Rect(board_render.surface.get_rect())
  rect.center = (0,0)

  side_calculation = (SMALL_TILE_SIZE[1] + 10) * 3
  if player_id == 0: rect.bottom = side_calculation + TILE_SIZE[1] + 20
  if player_id == 2: rect.top = -side_calculation

  tile_offset = 10
  tiles_per_row = 12
  i = 0
  row = 0
  row_offset = SMALL_TILE_SIZE[1] + tile_offset
  full_width = tiles_per_row * (SMALL_TILE_SIZE[0] + tile_offset) - tile_offset
  beginning_of_across_line = (rect.width - full_width) / 2
  beginning_of_across_line += full_width if player_id == 2 else 0
  across = beginning_of_across_line
  for tile in player.discard_pile:
    tile_pos = (across, -rect.y + (row * row_offset))
    tile_sprite = TileRender(board_render.small_dictionary, tile, tile_pos, small_tile=True, rotation = player_id)
    group.add(tile_sprite)
    across += SMALL_TILE_SIZE[0] + tile_offset if player_id == 0 else -(SMALL_TILE_SIZE[0] + tile_offset)
    i += 1
    if i >= tiles_per_row:
      i = 0
      row += 1 if player_id == 0 else -1
      across = beginning_of_across_line
  return group

def render_vertical_discard_pile(board_render, player_id):
  group = Group()
  board = board_render.board
  player = board.players[player_id]

  if player.discard_pile is None:
    return group

  rect = Rect(board_render.surface.get_rect())
  rect.center = (0,0)

  side_calculation = (SMALL_TILE_SIZE[1] + 10) * 4
  if player_id == 1: rect.right = side_calculation + 150
  if player_id == 3: rect.left = -side_calculation - 150

  tile_offset = 10
  tiles_per_row = 12
  i = 0
  row = 0
  row_offset = SMALL_TILE_SIZE[1] + tile_offset
  full_width = tiles_per_row * (SMALL_TILE_SIZE[0] + tile_offset) - tile_offset
  beginning_of_across_line = rect.height - ((rect.height - full_width) / 2)
  beginning_of_across_line -= full_width if player_id == 3 else 0
  across = beginning_of_across_line
  for tile in player.discard_pile:
    tile_pos = (-rect.x + (row * row_offset), across)
    tile_sprite = TileRender(board_render.small_dictionary, tile, tile_pos, small_tile=True, rotation = player_id)
    group.add(tile_sprite)
    across -= SMALL_TILE_SIZE[0] + tile_offset if player_id == 1 else -(SMALL_TILE_SIZE[0] + tile_offset)
    i += 1
    if i >= tiles_per_row:
      i = 0
      row += 1 if player_id == 1 else -1
      across = beginning_of_across_line
  return group