from pygame.sprite import Sprite

class TileRender(Sprite):
  def __init__(self, dictionary, tile, pos=(0,0)):
    Sprite.__init__(self)
    self.image = dictionary[tile]
    self.rect = pos