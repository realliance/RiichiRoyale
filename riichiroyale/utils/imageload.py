import pygame

def load_image(name, colorkey=None, convert=True):
    try:
        image = pygame.image.load(name)
    except pygame.error() as message:
        print('Cannot load image:', name)
        raise SystemExit from message
    if convert:
      image = image.convert()
      if colorkey is not None:
          if colorkey == -1:
              colorkey = image.get_at((0,0))
          image.set_colorkey(colorkey, pygame.RLEACCEL)
    return image