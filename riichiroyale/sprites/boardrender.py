import copy
from functools import reduce
from pygame.sprite import Group
from pygame import Rect, transform
from pygame.surface import Surface
import pygame
from riichiroyale.game import Tile, CallDirection
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

    # Player Hand
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].hand, lambda: render_hand(self, player_pov)))

    # Player Melds
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].melded_hand, lambda: render_meld_hand(self, self.board.players[player_pov].melded_hand, len(self.board.players[player_pov].hand))))

    # Discard Piles
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[0].discard_pile, lambda: render_discard_pile(self, 0)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[1].discard_pile, lambda: render_vertical_discard_pile(self, 1)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[2].discard_pile, lambda: render_discard_pile(self, 2)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[3].discard_pile, lambda: render_vertical_discard_pile(self, 3)))

    # Doras
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

def render_meld_hand(board_render, meld_hand, player_tiles):
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
  print(xpos, ypos)
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
