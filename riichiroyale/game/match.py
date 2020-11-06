from threading import Thread, Condition
from libmahjong import Wind, start_game, Piece, PieceType, GameSettings
from .board import Board
from .player import Player
from .boardmanager import process_event_queue


class Match(Thread):
    def __init__(
        self, ai_list, game_manager, player_manager=None, sound_manager=None, two_wind_game=False, starting_score=25000
    ):
        super().__init__()
        self.ai_list = ai_list
        self.players = []
        self.player_manager = player_manager
        player_manager.current_match = self
        self.sound_manager = sound_manager
        self.game_manager = game_manager
        self.scores = [starting_score] * 4
        self.current_board = None
        self.east_prevalent = True
        self.two_wind_game = two_wind_game
        self.match_ready = False
        self.match_alive = True
        self.process_lock = Condition()

    def new_board(self, wall=None, deadwall=None):
        self.current_board = None
        self.current_board = Board(
            wall=wall,
            deadwall=deadwall,
            dora_revealed=0,
        )

    def play_clack(self):
        if self.sound_manager is not None:
            self.sound_manager.play_from_set("clack")

    def bootstrap_match(self):
        for i in range(4):
            if i == self.player_manager.player_id:
                self.players += [self.player_manager]
            else:
                self.players += [Player("Bot {}".format(i), starting_hand=[Piece(PieceType.ERROR)] * 13, player_id=i)]
        self.new_board()
        self.match_ready = True
    
    def run(self):
        settings = GameSettings()
        settings.seat_controllers = self.ai_list
        start_game(settings, True)
        while self.match_alive:
            self.on_update()

    def on_update(self):
        if not self.match_ready and self.player_manager.player_id is None:
            return
        if not self.match_ready and self.player_manager.player_id is not None:
            print("Player ID Found, bootstrapping...")
            self.bootstrap_match()

        with self.process_lock:
            self.process_lock.wait_for(lambda: self.player_manager.GetQueueLength() > 0)
            process_event_queue(self.game_manager, self)





