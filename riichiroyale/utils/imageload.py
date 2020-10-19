import pygame
from pygame.transform import smoothscale

def load_image(name, surface, size=None, position=(0,0)):
  try:
    image = pygame.image.load(name)
  except pygame.error() as message:
    print('Cannot load image:', name)
    raise SystemExit from message
  if size is not None:
    image = smoothscale(image, size)
  surface.blit(image, position)
