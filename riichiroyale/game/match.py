from threading import Thread, Condition, Lock
from time import sleep
import numpy
from libmahjong import Wind, start_game, Piece, PieceType, GameSettings, Wind, halt_game
from .board import Board
from .player import Player
from .boardmanager import process_event_queue
from .meld import Meld
from .call import CallDirection


class Match(Thread):
    def __init__(
        self, ai_list, game_manager, player_manager=None, sound_manager=None, two_wind_game=False, starting_score=25000, wall=None, deadwall=None
    ):
        super().__init__()
        self.ai_list = ai_list
        self.players = []
        self.player_manager = player_manager
        player_manager.current_match = self
        self.sound_manager = sound_manager
        self.game_manager = game_manager
        self.scores = [starting_score] * 4
        self.delta_scores = [0] * 4
        self.current_board = None
        self.east_prevalent = True
        self.round_number = 0
        self.two_wind_game = two_wind_game
        self.match_ready = False
        self.match_alive = True
        self.process_lock = Condition()
        self.match_lock = Lock()
        self.game_id = -1
        self.current_board = None
        if wall is not None and deadwall is not None:
            self.current_board = Board(
                wall=wall,
                deadwall=deadwall,
                dora_revealed=0,
            )

    def new_board(self, wall=None, deadwall=None):
        self.current_board = None
        self.current_board = Board(
            wall=wall,
            deadwall=deadwall,
            dora_revealed=0,
        )
        self.current_board.current_dealer = (self.round_number - 1) % 4

    def play_clack(self):
        if self.sound_manager is not None:
            self.sound_manager.play_from_set("clack")

    def bootstrap_match(self):
        for i in range(4):
            if i == self.player_manager.player_id:
                self.players += [self.player_manager]
            else:
                self.players += [Player("Bot {}".format(i), starting_hand=[Piece(PieceType.ERROR)] * 13, player_id=i)]
        self.round_number = 1
        self.new_board()

        self.match_ready = True
    
    def run(self):
        settings = GameSettings()
        settings.seat_controllers = self.ai_list
        if self.current_board is not None:
            wall = list(map(Piece, self.current_board.wall+self.current_board.deadwall))
            settings.override_wall = wall
        self.game_id = start_game(settings, True)
        while self.match_alive:
            self.on_update()
        print('Match Halting...')
        halt_game(self.game_id)

    def start_next_round(self):
        self.scores = list(map(lambda x: x[0] + x[1], zip(self.scores, self.delta_scores)))
        winner_indices = numpy.argwhere(numpy.array(self.delta_scores) > 0).flatten()
        self.delta_scores = [0] * 4
        self.player_manager.reset()
        self.player_manager.next_round()
        if self.current_board.current_dealer not in winner_indices:
            self.round_number += 1
            self.east_prevalent = not (self.east_prevalent and self.player_manager.prevalent_wind != Wind.East)
        self.new_board()
        for i in range(4):
            if i != self.player_manager.player_id:
                self.players[i].reset()
                self.players[i].hand = [Piece(PieceType.ERROR)] * 13
        self.game_manager.board_manager.did_exhaustive_draw = True
        self.game_manager.board_manager.round_should_end = False

    def on_update(self):
        if not self.match_ready and self.player_manager.player_id is None:
            return
        if not self.match_ready and self.player_manager.player_id is not None:
            print("Player ID Found, bootstrapping...")
            self.bootstrap_match()

        with self.process_lock:
            self.process_lock.wait_for(lambda: self.player_manager.GetQueueLength() > 0, timeout=500)
            with self.match_lock:
                process_event_queue(self.game_manager, self)
        
        if self.game_manager.board_manager.round_should_end:
            sleep(7)
            self.start_next_round()





