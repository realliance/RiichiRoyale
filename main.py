import sys
import pygame
import libmahjong

SCREEN_WIDTH, SCREEN_HEIGHT = 1500, 400

def load_tile(foreground, background, colorkey=None):
    merged = background.copy()
    merged.blit(foreground, (0, 0))
    merged = merged.convert()
    if colorkey != None:
        if colorkey == -1:
            colorkey = merged.get_at((0,0))
        merged.set_colorkey(colorkey, pygame.constants.RLEACCEL)
    return merged

def load_image(name, colorkey=None, convert=True):
    try:
        image = pygame.image.load(name)
    except pygame.error as message:
        print('Cannot load image:', name)
        raise SystemExit(message)
    if convert == True:
      image = image.convert()
      if colorkey != None:
          if colorkey == -1:
              colorkey = image.get_at((0,0))
          image.set_colorkey(colorkey, pygame.constants.RLEACCEL)
    return image

class MahjongPiece(pygame.sprite.Sprite):
  def __init__(self, image, posx=0, posy=0):
    pygame.sprite.Sprite.__init__(self)
    self.image = image
    self.rect = posx, posy

class MahjongHand(pygame.sprite.Group):
  def __init__(self):
    pygame.sprite.Group.__init__(self)
    self.front = load_image('resources/tiles/102x136/Front.png', convert=False)
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

def main():
    # Initialise screen
    pygame.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption('Riichi Royale')

    # Fill background
    background = pygame.Surface(screen.get_size())
    background = background.convert_alpha()
    background.fill((255, 255, 255))

    # Display some text
    font = pygame.font.Font('extlibs/source-sans-pro/TTF/SourceSans3-Bold.ttf', 36)
    text = font.render("Press Space to Refresh Hand", 1, (10, 10, 10))
    textpos = text.get_rect()
    textpos.bottomright = background.get_rect().bottomright
    background.blit(text, textpos)

    libmahjong.Walls.SetPath('resources/tiles/102x136')
    hand = MahjongHand()

    # Blit everything to the screen
    screen.blit(background, (0, 0))
    pygame.display.flip()

    space_pressed = False

    # Event loop
    while 1:
        pygame.event.pump()
        keys = pygame.key.get_pressed()
        for event in pygame.event.get():
            if event.type == pygame.constants.QUIT:
                return
        if keys[pygame.constants.K_SPACE] and not space_pressed:
          space_pressed = True
          hand.newHand()
        if not keys[pygame.constants.K_SPACE]:
          space_pressed = False
        screen.blit(background, (0, 0))
        hand.update()
        hand.draw(screen)
        pygame.display.flip()
        pygame.display.update()

if __name__ == "__main__":
  sys.exit(main())
