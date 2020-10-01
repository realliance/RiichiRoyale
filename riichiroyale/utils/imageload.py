import pygame
from svg import Parser, Rasterizer

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

def load_svg(filename, surface, size, position=(0,0)):
    svg = Parser.parse_file(filename)
    w, h = size
    scale_w = float(w) / svg.width
    scale_h = float(h) / svg.height
    scale = min([scale_w, scale_h])
    rast = Rasterizer()
    buff = rast.rasterize(svg, w, h, scale)
    image = pygame.image.frombuffer(buff, (w, h), 'RGBA')
    surface.blit(image, position)
