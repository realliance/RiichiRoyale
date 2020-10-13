from .board import Board

class Match():
  def __init__(self, sound_manager=None, two_wind_game=False):
    self.players = []
    self.sound_manager = sound_manager
    self.scores = dict()
    self.current_board = None
    self.current_dealer = 0
    self.east_prevalent = True
    self.two_wind_game = two_wind_game

  def register_player(self, player, starting_score=25000, seat=None):
    if seat is not None:
      if self.players[seat] is not None:
        raise "Attempted to register player to an already taken seat"
      self.players[seat] = player
    else:
      self.players += [player]
    self.scores[player.name] = starting_score

  def increment_match(self):
    self.current_dealer += 1
    if self.current_dealer > 3 and self.east_prevalent:
      self.current_dealer = 0
      self.east_prevalent = False

  def new_board(self):
    self.current_board = Board(sound_manager=self.sound_manager, current_dealer=self.current_dealer, players=self.players)
    for player in self.players:
      player.board = self.current_board
      player.hand = self.current_board.draw_tile(num=13)
      player.hand.sort()

  def should_end(self):
    # Game should end if the dealer has revolved and either a) it is a single wind game or b) it is a two wind game and south is the prevalent wind
    return not self.east_prevalent and not self.two_wind_game or (self.current_dealer > 3 and not self.east_prevalent and self.two_wind_game)