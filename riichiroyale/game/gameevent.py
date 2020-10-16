from enum import IntEnum

class EventType(IntEnum):
  Ron = 0
  Kan = 1
  Pon = 2
  Chi = 3
  Tsumo = 4
  ConcealedKan = 5
  Riichi = 6
  Discard = 7
  Decline = 8
  Dora = 9
  PointDiff = 10
  End = 11

class CombinedEventType(IntEnum):
  DiscardRon = 100
  DiscardKan = 101
  DiscardPon = 102
  DiscardChi = 103

class Event():
  def __init__(self, event_type, player, piece, decision, extra_player=None, extra_piece=None):
    self.type = event_type
    self.player = player
    self.piece = piece
    self.extra_player = extra_player
    self.extra_piece = extra_piece
    self.decision = decision
  

