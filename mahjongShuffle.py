import sys
import sdl2
import sdl2.ext
import sdl2.sdlimage
import sdl2.sdlttf
import c_mahjong

class SoftwareRenderer(sdl2.ext.SoftwareSpriteRenderSystem):
  def __init__(self, window):
    super(SoftwareRenderer, self).__init__(window)

  def render(self, components):
    sdl2.ext.fill(self.surface, sdl2.ext.Color(255, 255, 255))
    super(SoftwareRenderer, self).render(components)

class Instructions(sdl2.ext.Entity):
  def __init__(self, world, sprite, posx=0, posy=0):
    self.sprite = sprite
    self.sprite.position = posx, posy

class MahjongPiece(sdl2.ext.Entity):
  def __init__(self, world, sprite, posx=0, posy=0):
    self.sprite = sprite
    self.sprite.position = posx, posy

class MahjongHand:
  def __init__(self, world, factory):
    self.pieces = []
    self.front = sdl2.ext.load_image('sprites/102x136/Front.png')
    self.bounds = sdl2.SDL_Rect(0,0,self.front.w,self.front.h)
    self.factory = factory
    posx = 0
    for piece in self.getPieces():
      self.pieces += [MahjongPiece(world, self.createSprite(piece), posx)]
      posx += self.pieces[-1].sprite.size[0]+10

  def getPieces(self):
    w = c_mahjong.Walls()
    p = w.TakeHand()
    return sorted(p)

  def newHand(self):
    for i,piece in enumerate(self.getPieces()):
      position = self.pieces[i].sprite.position 
      self.pieces[i].sprite = self.createSprite(piece)
      self.pieces[i].sprite.position = position
      
  def createSprite(self,piece):
    tile = sdl2.ext.load_image(c_mahjong.Walls.GetPath(piece).split('.')[0]+'.png')
    composite = sdl2.SDL_CreateRGBSurface(0,self.front.w,self.front.h,self.front.format.contents.BitsPerPixel,0,0,0,2**32-1)
    sdl2.SDL_BlitSurface(self.front,self.bounds,composite,self.bounds)
    sdl2.SDL_BlitSurface(tile,self.bounds,composite,self.bounds)
    return self.factory.from_surface(composite.contents)
    

def createInstructions():
  font = sdl2.sdlttf.TTF_OpenFont("source-sans-pro/TTF/SourceSans3-Regular.ttf".encode('utf-8'),72)
  return sdl2.sdlttf.TTF_RenderText_Solid(font, "Press Space to get a new hand.".encode("utf-8"),sdl2.SDL_Color(0,0,0))

# def GetHand(world,factory):
#   w = c_mahjong.Walls()
#   pieces = w.TakeHand()
#   pieces = sorted(pieces)
#   piece1 = MahjongPiece(world, factory.from_image("riichi-mahjong-tiles/Regular/Front.svg"), 0, 250)

def main():
  sdl2.ext.init()
  sdl2.sdlttf.TTF_Init()
  window = sdl2.ext.Window("Mahjong Shuffle", size=(1568, 300))
  window.show()

  world = sdl2.ext.World()

  spriterenderer = SoftwareRenderer(window)
  world.add_system(spriterenderer)

  factory = sdl2.ext.SpriteFactory(sdl2.ext.SOFTWARE)

  c_mahjong.Walls.SetPath('sprites/102x136')
  hand = MahjongHand(world,factory)

  instructions = Instructions(world, factory.from_surface(createInstructions().contents),0,200)
  
  running = True
  while running:
    events = sdl2.ext.get_events()
    for event in events:
      if event.type == sdl2.SDL_KEYDOWN:
        if event.key.keysym.sym == sdl2.SDLK_SPACE:
          hand.newHand()
      if event.type == sdl2.SDL_QUIT:
        running = False
        break
    world.process()

if __name__ == "__main__":
  sys.exit(main())
