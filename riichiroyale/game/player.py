from .call import Call, chi_possible, kan_possible, pon_possible

class Player:
  def __init__(self, name, starting_hand=None, discard_pile=None):
    self.name = name
    self.hand = starting_hand
    self.melded_hand = []
    self.discard_pile = discard_pile
    self.hand_open = False
    self.riichi_declared = False
    self.calls_avaliable = None

  def full_hand(self):
    return self.hand + self.melded_hand

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
    else:
      self.calls_avaliable = calls_possible
      return True

  def on_turn(self, board):
    self.hand.append(board.draw_tile())