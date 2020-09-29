from pygame.sprite import Sprite

class MahjongPiece(Sprite):
  def __init__(self, image, posx=0, posy=0):
    Sprite.__init__(self)
    self.image = image
    self.rect = posx, posy