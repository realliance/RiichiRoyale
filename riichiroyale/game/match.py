from libmahjong import PythonAIInterface, Wind
from .board import Board

class Match():
  def __init__(self, player_id, sound_manager=None, two_wind_game=False, ai_managed=False):
    self.players = []
    self.player_id = player_id
    print('match init')
    self.sound_manager = sound_manager
    self.scores = []
    self.current_board = None
    self.current_dealer = 0
    self.east_prevalent = True
    self.ai_managed = ai_managed
    self.player_ai_inst = None
    self.two_wind_game = two_wind_game

  def register_player(self, player, starting_score=0, seat=None):
    if seat is not None:
      if self.players[seat] is not None:
        raise "Attempted to register player to an already taken seat"
      self.players[seat] = player
    else:
      self.players += [player]
      player.player_id = len(self.players) - 1
    self.scores += [starting_score]

  def increment_match(self):
    self.current_dealer += 1
    if self.current_dealer > 3 and self.east_prevalent:
      self.current_dealer = 0
      self.east_prevalent = False

  def new_board(self, wall=None, deadwall=None):
    self.current_board = None
    self.current_board = Board(sound_manager=self.sound_manager, wall=wall, deadwall=deadwall, current_dealer=self.current_dealer, players=self.players, dora_revealed=0 if self.ai_managed else 1, ai_managed=self.ai_managed)
    for player in self.players:
      player.board = self.current_board
      if not self.ai_managed:
        player.hand = self.current_board.draw_tile(num=13)
        player.hand.sort()
      else:
        round_info = PythonAIInterface.Inst().RoundStart()
        player.hand = round_info.hand
        player.hand.sort()
        self.current_board.current_dealer = round_info.seat_wind
        self.east_prevalent = round_info.prevalent_wind == Wind.East

  def should_end(self):
    # Game should end if the dealer has revolved and either a) it is a single wind game or b) it is a two wind game and south is the prevalent wind
    return not self.east_prevalent and not self.two_wind_game or (self.current_dealer > 3 and not self.east_prevalent and self.two_wind_game)