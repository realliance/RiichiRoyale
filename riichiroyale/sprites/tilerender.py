from pygame.sprite import DirtySprite
from pygame import mouse, Rect, transform
import pygame
from .tileload import TILE_SIZE, SMALL_TILE_SIZE

class TileRender(DirtySprite):
  def __init__(self, dictionary, tile, pos, owner=None, known_index=None, small_tile=False, interact=False, rotation=0):
    DirtySprite.__init__(self)
    tile_surface = dictionary[tile].copy()
    tile_surface = transform.rotate(tile_surface, 90 * rotation)
    tile_surface = tile_surface.convert_alpha()
    self.image = tile_surface
    self.pos = pos
    self.rect = pos
    self.owner = owner
    self.known_index = known_index
    sizes = SMALL_TILE_SIZE if small_tile else TILE_SIZE
    self.tile_rect = Rect(self.rect[0], self.rect[1], sizes[0], sizes[1])
    self.can_interact = interact

  def update(self):
    if self.can_interact and self.tile_rect.collidepoint(mouse.get_pos()):
      self.rect = (self.pos[0], self.pos[1] - 10)
      self.dirty = 1
      buttons = mouse.get_pressed()
      if buttons[0] and self.owner is not None:
        self.owner.on_tile_click(self.known_index)
    else:
      self.rect = self.pos
      self.dirty = 1
