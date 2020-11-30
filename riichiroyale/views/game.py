from libmahjong import start_game, EventType, EngineEvent, PieceType
from riichiroyale.game import Player, Match, process_event_queue, DialogManager, loadStory, CombinedEventType
from .boardview import BoardView


class GameView(BoardView):
    def __init__(
        self,
        game_manager,
        screen,
        tile_dict,
        small_tile_dict,
        screen_width,
        screen_height,
        width_ratio,
        height_ratio,
        player_manager=None,
        name="game",
        textbox_hook=None
    ):
        super().__init__(
            name,
            game_manager,
            DialogManager(),
            screen,
            tile_dict,
            small_tile_dict,
            screen_width,
            screen_height,
            width_ratio,
            height_ratio,
            textbox_hook=textbox_hook
        )
        self.player_manager = player_manager
        self.ai_list = ["Player"] + ["AngryDiscardoBot"] * 3
        self.end_game = False
        self.match = None

    def on_match_init(self):
        if self.match is not None:
            self.match.match_alive = False
            self.match = None
        self.match = Match(self.ai_list, self.game_manager, self.player_manager, self.sound_manager)
        self.match.start()

    def on_pov_init(self):
        if self.player_manager is None:
            self.match_pov = 0

    def on_tile_pressed(self, owner, tile_hand_index):
        if owner.my_turn:
            tile = owner.hand[tile_hand_index]
            if (
                self.game_manager.board_manager.waiting_on_decision
                and len(owner.calls_avaliable) == 0
            ):
                event = EngineEvent()
                event.type = EventType.Discard
                event.piece = int(tile.get_raw_value())
                event.player = owner.player_id
                event.decision = True
                self.player_manager.MakeDecision(event)

    def on_pon_button_pressed(self):
        event = EngineEvent()
        event.type = EventType.Pon
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)  
        return True

    def on_chi_button_pressed(self):
        event = EngineEvent()
        event.type = EventType.Chi
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)  
        return True

    def on_kan_button_pressed(self):
        event = EngineEvent()
        event.type =  self.game_manager.board_manager.last_decision_event.raw_event_b.type if self.game_manager.board_manager.last_decision_event.type == CombinedEventType.DiscardKan else self.game_manager.board_manager.last_decision_event.type
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)  
        return True

    def on_tsumo_button_pressed(self):
        event = EngineEvent()
        event.type = EventType.Tsumo
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)  

        return True

    def on_riichi_button_pressed(self):
        event = EngineEvent()
        event.type = EventType.Riichi
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)  

        return True

    def on_ron_button_pressed(self):
        event = EngineEvent()
        event.type = EventType.Ron
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)     

        return True

    def on_skip_button_pressed(self):
        event = EngineEvent()
        event.type = EventType.Decline
        event.piece = -1
        event.player = -1
        event.decision = True

        self.match.player_manager.MakeDecision(event)            

        return True

    def on_dialogue_event_ending(self, event_name):
        if event_name == "round_end":
            self.game_manager.set_active_view("main_menu")
            self.game_manager.sound_manager.play_music("lobby")
        self.lock_user_input = False

    def _end_round_dialog(self):
        # Skip for Own Round Handling
        pass

    def update(self, time_delta):
        if self.game_manager.board_manager.round_should_end:
            self._end_round_dialog()

        if self.game_manager.board_manager.game_should_end:
            self.game_manager.board_manager.game_should_end = False
            # Use own flag to ensure inheritance does not mess with game ending
            self.end_game = True

        super().update(time_delta)

        if self.end_game:
            self.end_game = False
            self.game_manager.set_active_view("main_menu")
            self.game_manager.sound_manager.play_music("lobby")
