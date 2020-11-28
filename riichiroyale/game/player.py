
class Player:
    def __init__(self, name, starting_hand=None, discard_pile=None, ai_managed=False, player_id=None):
        self.name = name
        self.player_id = player_id
        if starting_hand is None:
            self.hand = []
        else:
            self.hand = starting_hand
        self.melded_hand = []
        if discard_pile is None:
            self.discard_pile = []
        else:
            self.discard_pile = discard_pile
        self.hand_open = False
        self.board = None
        self.riichi_declared = False
        self.calls_avaliable = []
        self.my_turn = False
        self.discarder = None
        self.ai_managed = ai_managed

    def full_hand(self):
        meld_tiles = len(self.melded_hand) * 3
        return self.hand + meld_tiles

    def reset(self):
        self.discard_pile = []
        self.melded_hand = []
        self.hand = []
        self.riichi_declared = False
        self.hand_open = False
