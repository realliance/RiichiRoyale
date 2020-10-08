from pygame.surface import Surface
from pygame import Rect
import pygame
from pygame.sprite import LayeredUpdates, Sprite
from pygame.font import Font

def create_wind_markers(board, parent_rect, group):
  font = Font("build/_deps/sourcesanspro-src/TTF/SourceSans3-Semibold.ttf", 40)
  MARKER_SIZE = (40, 40)
  MARKER_SIZE_HALF = (MARKER_SIZE[0] / 2, MARKER_SIZE[1] / 2)
  MARKER_OFFSET = 40
  parent_half_width = parent_rect.width / 2
  parent_half_height = parent_rect.height / 2
  SEAT_OFFSETS = [
    (0, parent_half_height - MARKER_SIZE_HALF[1] - MARKER_OFFSET),
    (parent_half_width - MARKER_SIZE_HALF[0] - MARKER_OFFSET, 0),
    (0, -parent_half_height + MARKER_SIZE_HALF[1] + MARKER_OFFSET),
    (-parent_half_width + MARKER_SIZE_HALF[0] + MARKER_OFFSET, 0),
  ]
  WIND_ORDERS = [["E", "S","W","N"], ["N", "E","S","W"], ["W", "N","E","S"], ["S", "W","N","E"]]
  current_east_seat = board.current_dealer
  wind_order = WIND_ORDERS[current_east_seat]

  for i in range(4):
    wind = wind_order[i]
    background_surface = Surface(MARKER_SIZE)
    if i == current_east_seat:
      background_color = (255, 0, 0)
    else:
      background_color = (100, 100, 100)
    background_surface.fill(background_color)
    font_surface = font.render(wind, True, (255, 255, 255))
    font_width, font_height = font.size(wind)
    background_surface.blit(font_surface, ((MARKER_SIZE[0] - font_width) / 2, (MARKER_SIZE[1] - font_height) / 2))

    sprite = Sprite()
    sprite.rect = background_surface.get_rect()
    sprite.image = background_surface
    sprite.rect.center = parent_rect.center
    sprite.rect.x += SEAT_OFFSETS[i][0]
    sprite.rect.y += SEAT_OFFSETS[i][1]
    sprite.layer = 2
    group.add(sprite)

def create_turn_marker(board, parent_rect, group):
  MARKER_OFFSET = 20
  MARKER_WIDTH = 5
  current_turn = board.current_turn
  if current_turn % 2 == 0:
    marker_size = (parent_rect.width - MARKER_OFFSET, MARKER_WIDTH)
  else:
    marker_size = (MARKER_WIDTH, parent_rect.width - MARKER_OFFSET)
  marker_size_half = (marker_size[0] / 2, marker_size[1] / 2)
  parent_half_width = parent_rect.width / 2
  parent_half_height = parent_rect.height / 2
  SEAT_OFFSETS = [
    (0, parent_half_height - marker_size_half[1]),
    (parent_half_width - marker_size_half[0], 0),
    (0, -parent_half_height + marker_size_half[1]),
    (-parent_half_width + marker_size_half[0], 0)
  ]

  turn_marker_surface = Surface(marker_size)
  turn_marker_surface.fill((255, 255, 255))
  sprite = Sprite()
  sprite.rect = turn_marker_surface.get_rect()
  sprite.image = turn_marker_surface
  sprite.rect.center = parent_rect.center
  sprite.rect.x += SEAT_OFFSETS[current_turn][0]
  sprite.rect.y += SEAT_OFFSETS[current_turn][1]
  sprite.layer = 2
  group.add(sprite)

def create_riichi_markers(board, parent_rect, group):
  MARKER_OFFSET = 60
  MARKER_Y_OFFSET = 10
  MARKER_WIDTH = 15

  i = -1
  for player in board.players:
    i += 1
    if not player.riichi_declared:
      continue

    if i % 2 == 0:
      marker_size = (parent_rect.width - MARKER_OFFSET, MARKER_WIDTH)
    else:
      marker_size = (MARKER_WIDTH, parent_rect.width - MARKER_OFFSET)
    marker_size_half = (marker_size[0] / 2, marker_size[1] / 2)
    parent_half_width = parent_rect.width / 2
    parent_half_height = parent_rect.height / 2
    SEAT_OFFSETS = [
      (0, parent_half_height - marker_size_half[1] - MARKER_Y_OFFSET),
      (parent_half_width - marker_size_half[0] - MARKER_Y_OFFSET, 0),
      (0, -parent_half_height + marker_size_half[1] + MARKER_Y_OFFSET),
      (-parent_half_width + marker_size_half[0] + MARKER_Y_OFFSET, 0)
    ]

    riichi_marker_surface = Surface(marker_size)
    marker_rect = riichi_marker_surface.get_rect()
    riichi_marker_surface.fill((255, 255, 255))
    pygame.draw.circle(riichi_marker_surface, (255, 0, 0), (marker_rect.width / 2, marker_rect.height / 2), 5)
    sprite = Sprite()
    sprite.rect = marker_rect
    sprite.image = riichi_marker_surface
    sprite.rect.center = parent_rect.center
    sprite.rect.x += SEAT_OFFSETS[i][0]
    sprite.rect.y += SEAT_OFFSETS[i][1]
    sprite.layer = 2
    group.add(sprite)

def render_center_info(board_render):
  center_info_surface = Surface((250, 250))
  board = board_render.board
  group = LayeredUpdates()

  # Background
  info_rect = Rect(0, 0, 250, 250)
  info_rect.center = board_render.surface.get_rect().center
  pygame.draw.rect(center_info_surface, (0, 0, 0), info_rect)

  font = Font("build/_deps/sourcesanspro-src/TTF/SourceSans3-Semibold.ttf", 60)

  # Tiles Left in Wall
  tile_count_surface = font.render(str(len(board.wall)), True, (255, 255, 255))
  wall_remaining_sprite = Sprite()
  wall_remaining_sprite.image = tile_count_surface
  wall_remaining_sprite.rect = tile_count_surface.get_rect()
  wall_remaining_sprite.rect.center = info_rect.center
  wall_remaining_sprite.layer = 1
  group.add(wall_remaining_sprite)

  # Wind Markers
  create_wind_markers(board, info_rect, group)

  # Turn Marker
  create_turn_marker(board, info_rect, group)

  # Riichi Markers
  create_riichi_markers(board, info_rect, group)

  background_sprite = Sprite()
  background_sprite.rect = info_rect
  background_sprite.image = center_info_surface
  background_sprite.layer = 0
  group.add(background_sprite)
  return group