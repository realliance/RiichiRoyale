
class Board():
  def __init__(self, wall=[], deadwall=[], dora_revealed=0, players=[], current_turn=0, current_dealer=0):
    self.wall = wall
    self.deadwall = deadwall
    self.dora_revealed = dora_revealed
    self.players = players
    self.current_turn = current_turn
    self.current_dealer = current_dealer

  def next_turn(self):
    player_count = len(self.players)
    if player_count == 0:
      raise "Attempted to call next turn with no players!"
    
    self.current_turn += 1
    if self.current_turn >= player_count:
      self.current_turn - player_count
    
    self.player_count[self.current_turn].on_turn(self)

