from pygame.sprite import Group
from pygame import Rect, transform
from pygame.surface import Surface
import pygame
import copy
from riichiroyale.game import Tile
from riichiroyale.sprites import TileRender
from .tileload import TILE_SIZE, SMALL_TILE_SIZE

class StatefulBoardElement():
  def __init__(self, default_value, get_value, render):
    self.value = copy.deepcopy(default_value)
    self.get_value = get_value
    self.render = render
    self.rendered = Group()

  def notify(self, surface, background):
    if self.value != self.get_value():
      self.rendered.clear(surface, background)
      self.value = copy.deepcopy(self.get_value())
      self.rendered = self.render()
  
  def force_render(self):
    self.rendered = self.render()

  def draw(self, surface, background):
    self.rendered.clear(surface, background)
    self.rendered.draw(surface)

  def update(self):
    self.rendered.update()

class BoardRender():
  def __init__(self, small_dictionary, dictionary, surface, board, player_pov):
    self.board = board
    self.surface = surface
    self.small_dictionary = small_dictionary
    self.dictionary = dictionary
    self.player_pov = player_pov
    self.elements = []

    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].hand, lambda: render_hand(self, player_pov)))

    # Discard Piles
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[0].discard_pile, lambda: render_discard_pile(self, 0)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[1].discard_pile, lambda: render_vertical_discard_pile(self, 1)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[2].discard_pile, lambda: render_discard_pile(self, 2)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[3].discard_pile, lambda: render_vertical_discard_pile(self, 3)))

    self.elements.append(StatefulBoardElement(0, lambda: self.board.dora_revealed, lambda: render_dora_pile(self)))
    
  def update(self):
    # Notify elements to check state changes
    for element in self.elements:
      element.update()
    
  def force_redraw(self):
    for element in self.elements:
      element.force_render()

  def draw(self, background):
    for element in self.elements:
      element.notify(self.surface, background)
      element.draw(self.surface, background)


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
  for tile in player.hand:
    if tile_index == 13:
      xpos += 3 * tile_offset
    tile_sprite = TileRender(board_render.dictionary, tile, (xpos, -rect.y), owner=player, known_index=tile_index, interact=True)
    group.add(tile_sprite)
    xpos += TILE_SIZE[0] + tile_offset
    tile_index += 1
  return group

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
  player = board.players[0]

  if player.discard_pile is None:
    return group

  rect = Rect(board_render.surface.get_rect())
  rect.center = (0,0)

  side_calculation = (SMALL_TILE_SIZE[1] + 10) * 4
  if player_id == 1: rect.right = side_calculation
  if player_id == 3: rect.left = -side_calculation

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

def render_dora_pile(board_render):
  group = Group()
  board = board_render.board
  dora_revealed = board.get_revealed_dora()
  dora_revealed += [Tile.ERROR_PIECE] * (5 - board.dora_revealed)

  tile_offset = 10
  full_width = (SMALL_TILE_SIZE[0] + tile_offset) * 5 - tile_offset
  xpos = 0

  rect = Rect(0, 0, full_width, SMALL_TILE_SIZE[1])
  rect.center = board_render.surface.get_rect().center
  for tile in dora_revealed:
    tile_pos = (rect.x + xpos, rect.y)
    tile_sprite = TileRender(board_render.small_dictionary, tile, tile_pos, small_tile=True)
    group.add(tile_sprite)
    xpos += tile_offset + SMALL_TILE_SIZE[0]
  return group
