
class Player:
  def __init__(self, name, starting_hand=[], discard_pile=[]):
    self.name = name
    self.starting_hand = starting_hand
    self.discard_pile = discard_pile

  def on_turn(self, board):
    return