class Meld:
  def __init__(self, tiles, call_direction, converted_kan=False):
    self.tiles = tiles
    self.call_direction = call_direction
    self.converted_kan = converted_kan

  def calculate_meld_sprite_dimensions(self, width, height, tile_offset = 5):
    w = (len(self.tiles) - 1 if self.converted_kan else len(self.tiles)) * (width + tile_offset) - tile_offset + (height - width)
    h = max(height, (2 * width + 5) if self.converted_kan else 0)
    return w, h