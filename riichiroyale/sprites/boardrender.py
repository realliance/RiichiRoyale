from pygame.sprite import Group
from pygame import Rect
from riichiroyale.sprites import TileRender
from .tileload import TILE_SIZE

class StatefulBoardElement():
  def __init__(self, default_value, get_value, render):
    self.value = default_value
    self.get_value = get_value
    self.render = render
    self.rendered = Group()

  def notify(self):
    if self.value != self.get_value():
      self.value = self.get_value()
      self.rendered = self.render()
  
  def force_render(self):
    self.rendered = self.render()

  def draw(self, surface, background):
    self.rendered.clear(surface, background)
    self.rendered.draw(surface)

  def update(self):
    self.rendered.update()

class BoardRender():
  def __init__(self, dictionary, surface, board, player_pov):
    self.board = board
    self.surface = surface
    self.dictionary = dictionary
    self.player_pov = player_pov
    self.elements = []

    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].hand, lambda: render_hand(self, player_pov)))
    
  def update(self):
    # Notify elements to check state changes
    for element in self.elements:
      element.notify()
      element.update()
    
  def force_redraw(self):
    for element in self.elements:
      element.force_render()

  def draw(self, background):
    for element in self.elements:
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
  for tile in player.hand:
    tile_sprite = TileRender(board_render.dictionary, tile, (xpos, -rect.y))
    group.add(tile_sprite)
    xpos += TILE_SIZE[0] + tile_offset

  return group