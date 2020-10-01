import random
from .wall import generate_start_board

class Board():
  def __init__(self, wall=None, deadwall=None, dora_revealed=1, players=None, current_turn=0, current_dealer=0):
    if wall is not None and deadwall is not None:
      self.wall = wall
      self.deadwall = deadwall
    else:
      self.wall, self.deadwall = generate_start_board()
    self.dora_revealed = dora_revealed
    if players is None:
      self.players = []
    self.current_turn = current_turn
    self.current_dealer = current_dealer

  def draw_tile(self, num=1):
    if self.wall is None:
      raise "Wall is currently none!"
    if len(self.wall) < num:
      raise "Attempted to draw too many tiles, Wall doesn't have that number left!"
    tiles_drawn = self.wall[:num]
    del self.wall[:num]
    return tiles_drawn

  def shuffle_dealer(self):
    self.current_dealer = random.randrange(0, len(self.players))
    self.current_turn = self.current_dealer

  def out_of_tiles(self):
    return len(self.wall) == 0

  def get_revealed_dora(self):
    dora = []
    for index in range(0, self.dora_revealed, 2):
      dora.append(self.deadwall[index])
    return dora

  def next_turn(self):
    player_count = len(self.players)
    if player_count == 0:
      raise "Attempted to call next turn with no players!"
    
    self.current_turn += 1
    if self.current_turn >= player_count:
      self.current_turn = self.current_turn - player_count

    if not self.out_of_tiles():
      self.on_turn()

  def register_player(self, player):
    player.board = self
    self.players.append(player)

  def on_call(self, player):
    self.current_turn = self.players.index(player)

  def on_discard(self, player):
    player_index = self.players.index(player)
    wait_on_decision = False
    for index in range(len(self.players)):
      if index == player_index:
        pass
      on_left = player_index + 1 == index if index != 0 else player_index - 3 == index
      result = self.players[index].on_opponent_discard(player, on_left)
      wait_on_decision = wait_on_decision or result
    if not wait_on_decision:
      self.next_turn()

  def on_turn(self):
    self.players[self.current_turn].on_turn(self)

