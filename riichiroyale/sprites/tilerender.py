from pygame.sprite import DirtySprite
from pygame import mouse, Rect, transform
from libmahjong import Piece
from .tileload import TILE_SIZE, SMALL_TILE_SIZE


class TileRender(DirtySprite):
    def __init__(
        self,
        dictionary,
        tile,
        pos,
        owner=None,
        known_index=None,
        small_tile=False,
        interact=False,
        rotation=0,
    ):
        if not hasattr(tile, 'get_raw_value'):
            print('WARNING: Just attempted to pass a PieceType to TileRender!')
            tile = Piece(int(tile))

        DirtySprite.__init__(self)
        tile_surface = dictionary[tile.get_raw_value()].copy()
        tile_surface = transform.rotate(tile_surface, 90 * rotation)
        tile_surface = tile_surface.convert_alpha()
        self.image = tile_surface
        self.pos = pos
        self.rect = pos
        self.owner = owner
        self.known_index = known_index
        self.currently_pressed = False
        sizes = SMALL_TILE_SIZE if small_tile else TILE_SIZE
        self.tile_rect = Rect(self.rect[0], self.rect[1], sizes[0], sizes[1])
        self.can_interact = interact

    def update(self, callback_handler=None):
        if self.can_interact and self.tile_rect.collidepoint(mouse.get_pos()):
            self.rect = (self.pos[0], self.pos[1] - 10)
            self.dirty = 1
            buttons = mouse.get_pressed()
            if buttons[0] and self.owner is not None:
                self.currently_pressed = True

            if not buttons[0] and self.currently_pressed:
                self.currently_pressed = False
                if callback_handler is not None:
                    callback_handler(self.owner, self.known_index)
        else:
            self.rect = self.pos
            self.dirty = 1
