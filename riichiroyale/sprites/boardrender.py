from pygame.sprite import Group
from riichiroyale.sprites import TileRender

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

  def draw(self, surface):
    self.rendered.draw(surface)

class BoardRender():
  def __init__(self, dictionary, surface, board, player_pov):
    self.board = board
    self.surface = surface
    self.dictionary = dictionary
    self.player_pov = player_pov
    self.elements = []
    self.rendered_elements = []

    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].hand, lambda: render_hand(self, player_pov)))
    
  def update(self):
    for element in self.elements:
      element.notify()

  def draw(self):
    for element in self.elements:
      element.draw(self.surface)

def render_hand(board_render, player_pov):
  group = Group()
  board = board_render.board
  player = board.players[player_pov]

  xpos = 0
  for tile in player.hand:
    tile_sprite = TileRender(board_render.dictionary, tile, (xpos, 10))
    group.add(tile_sprite)
    xpos += 80

  return group