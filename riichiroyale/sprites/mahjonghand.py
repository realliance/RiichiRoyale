from pygame.sprite import Group
from pygame import surface
import pygame
from riichiroyale.sprites import MahjongPiece
from riichiroyale.utils import load_image

import libmahjong

class MahjongHand(Group):
  def __init__(self):
    Group.__init__(self)
    self.front = load_image('resources/tiles/51x68/Front.png', convert=False)
    posx = 10
    posy = 50
    for piece in self.getPieces():
      self.add(MahjongPiece(self.createSprite(piece), posx, posy))
      posx += 105

  def getPieces(self):
    w = libmahjong.Walls()
    p = w.TakeHand()
    return sorted(p)

  def newHand(self):
    sprites = self.sprites()
    self.empty()
    for i,piece in enumerate(self.getPieces()):
      pos = sprites[i].rect
      self.add(MahjongPiece(self.createSprite(piece), posx=pos[0], posy=pos[1]))
            
  def createSprite(self,piece):
    foreground = load_image(libmahjong.Walls.GetPath(piece).split('.')[0]+'.png', convert=False)
    return load_tile(foreground, self.front)

def load_tile(foreground, background, colorkey=None):
    _, _, width, height = background.get_rect()
    merged = surface.Surface([width, height], pygame.SRCALPHA)
    merged.blit(background, (0, 0))
    merged.blit(foreground, (0, 0))
    merged = merged.convert_alpha()
    if colorkey is not None:
        if colorkey == -1:
            colorkey = merged.get_at((0,0))
        merged.set_colorkey(colorkey, pygame.RLEACCEL)
    return merged