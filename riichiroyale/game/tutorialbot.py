from .player import Player

class TutorialBot(Player):
    def __init__(self, name, starting_hand=None, discard_pile=None):
        super().__init__(name, starting_hand=None, discard_pile=None)

    def on_turn(self, board):
        self.my_turn = True
        self.hand += board.draw_tile()
        tile = self.hand[13]
        del self.hand[13]
        self.discard_pile.append(tile)
        self.board.on_discard(self)
        self.my_turn = False

    def on_opponent_discard(self, player, chi_avaliable):
        return False

    def make_decision(self, player):
        self.calls_avaliable = []

