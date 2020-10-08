from functools import reduce
from .call import Call, chi_possible, kan_possible, pon_possible

class Player:
  def __init__(self, name, starting_hand=None, discard_pile=None):
    self.name = name
    if starting_hand is None:
      self.hand = []
    else:
      self.hand = starting_hand
    self.melded_hand = []
    if discard_pile is None:
      self.discard_pile = []
    else:
      self.discard_pile = discard_pile
    self.hand_open = False
    self.board = None
    self.riichi_declared = False
    self.calls_avaliable = None
    self.my_turn = False

  def full_hand(self):
    meld_tiles = len(self.melded_hand) * 3
    return self.hand + meld_tiles

  def on_tile_click(self, tile_index):
    if self.my_turn:
      # Discard Tile
      tile = self.hand[tile_index]
      del self.hand[tile_index]
      self.hand.sort()
      self.discard_pile.append(tile)
      self.my_turn = False
      if self.board is None:
        raise "Cannot communicate with board! Is this player registered?"
      self.board.on_discard(self)

  def on_opponent_discard(self, player, chi_avaliable):
    if len(player.discard_pile) == 0:
      raise "Player does not have a discard pile even though on_opponent_discard was drawn!"
    tile_discarded = player.discard_pile[-1]
    calls_possible = []
    if chi_avaliable:
      calls_possible += [Call.Chi] if chi_possible(self.hand, tile_discarded) else []
    calls_possible += [Call.Pon] if pon_possible(self.hand, tile_discarded) else []
    calls_possible += [Call.Kan] if kan_possible(self.hand, tile_discarded) else []
    if len(calls_possible) == 0:
      self.calls_avaliable = None
      return False
    self.calls_avaliable = calls_possible
    return True

  def on_turn(self, board):
    self.my_turn = True
    self.hand += board.draw_tile()
