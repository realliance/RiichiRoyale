from pygame.sprite import DirtySprite
from pygame import mouse, Rect
from .tileload import TILE_SIZE

class TileRender(DirtySprite):
  def __init__(self, dictionary, tile, pos=(0,0)):
    DirtySprite.__init__(self)
    self.image = dictionary[tile]
    self.pos = pos
    self.rect = pos
    self.tile_rect = Rect(self.rect[0], self.rect[1], TILE_SIZE[0], TILE_SIZE[1])

  def update(self):
    if self.tile_rect.collidepoint(mouse.get_pos()):
      self.rect = (self.pos[0], self.pos[1] - 10)
      self.dirty = 1
    else:
      self.rect = self.pos
      self.dirty = 1
