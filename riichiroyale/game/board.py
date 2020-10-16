import random
from .wall import generate_start_board

class Board():
  def __init__(self, sound_manager=None, wall=None, deadwall=None, dora_revealed=1, players=None, current_turn=0, current_dealer=0):
    self.sound_manager = sound_manager
    if wall is not None and deadwall is not None:
      self.wall = wall
      self.deadwall = deadwall
    else:
      self.wall, self.deadwall = generate_start_board()
    self.dora_revealed = dora_revealed
    if players is None:
      self.players = []
    else:
      self.players = players
    self.current_turn = current_turn
    self.current_dealer = current_dealer
    self.decision_pending = False

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
    if self.sound_manager is not None:
      self.sound_manager.play_from_set('clack')
    player_index = self.players.index(player)
    for index in range(len(self.players)):
      if index == player_index:
        pass
      if self.players[index].name != 'Player':
        pass
      on_left = player_index + 1 == index if index != 0 else player_index - 3 == index
      if self.players[index].on_opponent_discard(player, on_left):
        self.decision_pending = True
    if not self.decision_pending:
      self.next_turn()
      

  def on_turn(self):
    self.players[self.current_turn].on_turn(self)

