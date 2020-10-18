import random
import asyncio
from .call import Call, CallDirection
from .meld import Meld
from .wall import generate_start_board
from .tile import Tile

class Board():
  def __init__(self, sound_manager=None, wall=None, deadwall=None, dora_revealed=1, players=None, ai_managed=False, current_turn=0, current_dealer=0):
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
    self.ai_managed = ai_managed

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

  def next_turn(self, player=None):
    player_count = len(self.players)
    if player_count == 0:
      raise "Attempted to call next turn with no players!"
    
    self.current_turn += 1
    if self.current_turn >= player_count:
      self.current_turn = self.current_turn - player_count

    if player is not None:
      self.current_turn = self.players.index(player)
      self.on_turn(draw=False)
      return

    if not self.out_of_tiles() and not self.ai_managed:
      self.on_turn()

  def register_player(self, player):
    player.board = self
    player.player_id = len(self.players)
    self.players.append(player)

  def on_call(self, player):
    self.current_turn = self.players.index(player)

  def play_clack(self):
    if self.sound_manager is not None:
          self.sound_manager.play_from_set('clack')

  def on_discard(self, player, is_tutorial_winning_tile=False):
    self.play_clack()
    player_index = self.players.index(player)
    for index in range(len(self.players)):
      if index == player_index:
        pass
      if self.players[index].name != 'Player':
        pass
      on_left = player_index + 1 == index if index != 0 else player_index - 3 == index
      ron_available=False
      if is_tutorial_winning_tile:
        ron_available=True
      if self.players[index].on_opponent_discard(player, on_left, ron_available=ron_available):
        self.decision_pending = True
    if not self.decision_pending:
      self.next_turn()
      
  def on_decision(self, call, player, discarder):
    if call == Call.Skip:
      self.next_turn()
      return

    if call == Call.Pon:
      pon_tile = discarder.discard_pile[-1]
      del discarder.discard_pile[-1]
      player.melded_hand += [Meld([pon_tile,pon_tile,pon_tile], CallDirection.get_call_direction(self.players.index(player),self.players.index(discarder)))]
      removed = 0
      tile_index = len(player.hand) - 1
      while tile_index >= 0:
        if (player.hand[tile_index] == pon_tile):
          del player.hand[tile_index]
          removed += 1
          tile_index = len(player.hand)
        tile_index -= 1
      self.next_turn(player)
      return

    if call == Call.Chi:
      chi_tile = discarder.discard_pile[-1]
      del discarder.discard_pile[-1]
      if Tile.getPieceNum(chi_tile) <= 7:
        if chi_tile + 1 in player.hand and chi_tile + 2 in player.hand:
          meld_peice1 = chi_tile + 1
          meld_peice2 = chi_tile + 2

      if not Tile.isTerminal(chi_tile):
        if chi_tile - 1 in player.hand and chi_tile + 1 in player.hand:
          meld_peice1 = chi_tile - 1
          meld_peice2 = chi_tile + 1

      if Tile.getPieceNum(chi_tile) >= 3:
        if chi_tile - 2 in player.hand and chi_tile - 1 in player.hand:
          meld_peice1 = chi_tile - 2
          meld_peice2 = chi_tile - 1

      direction = CallDirection.get_call_direction(self.players.index(player),self.players.index(discarder))
      if direction == 1:
        player.melded_hand += [Meld([chi_tile,meld_peice1,meld_peice2], direction)]
      elif direction == 2:
        player.melded_hand += [Meld([meld_peice1,chi_tile,meld_peice2], direction)]
      elif direction == 3:
        player.melded_hand += [Meld([meld_peice1,meld_peice2,chi_tile], direction)]

      tile_index = len(player.hand) - 1
      while tile_index >= 0:
        if player.hand[tile_index] == meld_peice1:
          del player.hand[tile_index]
          break
        tile_index -= 1

      tile_index = len(player.hand) - 1
      while tile_index >= 0:
        if player.hand[tile_index] == meld_peice2:
          del player.hand[tile_index]
          break
        tile_index -= 1
      self.next_turn(player)
      return
      

    self.next_turn()
      

  def on_turn(self, draw=True):
    self.players[self.current_turn].on_turn(self, draw=draw)

