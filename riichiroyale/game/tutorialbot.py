from .player import Player


class TutorialBot(Player):
    def __init__(
        self, name, tutorial_winning_call, starting_hand=None, discard_pile=None
    ):
        super().__init__(name, starting_hand=None, discard_pile=None)
        self.tutorial_winning_call = tutorial_winning_call

    def on_turn(self, board, draw=True):
        self.my_turn = True
        if draw:
            self.hand += board.draw_tile()
        tile = self.hand[13]
        del self.hand[13]
        self.discard_pile.append(tile)
        ron_available = tile == self.tutorial_winning_call
        self.my_turn = False
        self.board.on_discard(self, ron_available)

    def on_opponent_discard(self, player, chi_avaliable, ron_available=False):
        return False

    def make_decision(self, call):
        self.calls_avaliable = []
