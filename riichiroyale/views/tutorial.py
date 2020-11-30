from libmahjong import start_game, EventType, EngineEvent, PieceType
from riichiroyale.game import Player, Match, TutorialBot, Call, DialogManager, Piece
from riichiroyale.tutorial import Tutorial, get_wall, get_dialogue
from .boardview import BoardView


class TutorialView(BoardView):
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
        name="tutorial",
        player_manager=None,
    ):
        dialogue = get_dialogue()
        dialogue_manager = DialogManager()
        for item in dialogue:
            dialogue_manager.register_dialog_event(item)
            dialogue_manager.append_dialog_event(item, dialogue[item])

        super().__init__(
            name,
            game_manager,
            dialogue_manager,
            screen,
            tile_dict,
            small_tile_dict,
            screen_width,
            screen_height,
            width_ratio,
            height_ratio,
        )
        self.player_manager = player_manager
        self.tutorial = None
        self.ai_list = ["Player"] + ["TotoBot"] * 3

    def on_match_init(self):
        prefered_discards = [
            PieceType.SOUTH_WIND,
            PieceType.ONE_BAMBOO,
            PieceType.SEVEN_BAMBOO,
            PieceType.SEVEN_PIN,
            PieceType.ERROR,
        ]
        calls = ["pon", "chi", "skip", "ron", "endCall"]
        wall, deadwall = get_wall()

        self.tutorial = Tutorial(
            prefered_discards, calls, PieceType.THREE_PIN, wall, deadwall
        )
        del self.match
        self.match = self.match = Match(["Player"] + ["TotoBot"] * 3, self.game_manager, self.player_manager, self.sound_manager, wall=wall, deadwall=deadwall)
        self.match.start()
        self.pons = 0

    def on_pov_init(self):
        self.dialogue_manager.start_event('intro')

        #self.match.new_board(wall=self.tutorial.wall, deadwall=self.tutorial.deadwall)

    def on_tile_pressed(self, owner, tile_hand_index):
        tile = owner.hand[tile_hand_index]
        if owner.my_turn and Piece(self.tutorial.next_discard) == tile:
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
                self.tutorial.discard()

    def on_pon_button_pressed(self):
        if self.tutorial.next_call == "pon":
            self.tutorial.call()
            self.pons += 1
            self.dialogue_manager.start_event("after_pon")
            self.match.player_manager.MakeDecision(
                self.game_manager.board_manager.last_decision_event.raw_event_b
            )
            return True
        return False

    def on_pon_call_avaliable(self):
        if self.pons == 0:
            self.dialogue_manager.start_event("pon")
        elif self.pons == 1:
            self.dialogue_manager.start_event("skip_pon")

    def on_chi_button_pressed(self):
        if self.tutorial.next_call == "chi":
            self.tutorial.call()
            self.dialogue_manager.start_event("after_chi")
            self.match.player_manager.MakeDecision(
                self.game_manager.board_manager.last_decision_event.raw_event_b
            )
            return True
        return False

    def on_chi_call_avaliable(self):
        self.dialogue_manager.start_event("chi")

    def on_kan_button_pressed(self):
        raise "Kan Button Triggered in Tutorial."

    def on_tsumo_button_pressed(self):
        raise "Tsumo Button Triggered in Tutorial."

    def on_riichi_button_pressed(self):
        raise "Riichi Button Triggered in Tutorial."

    def on_skip_button_pressed(self):
        if self.tutorial.next_call == "skip":
            self.tutorial.call()
            event = EngineEvent()
            event.type = EventType.Decline
            event.piece = -1
            event.player = -1
            event.decision = True

            self.match.player_manager.MakeDecision(event)
            self.dialogue_manager.start_event("discard_tip")
            return True
        return False

    def on_ron_button_pressed(self):
        if self.tutorial.next_call == "ron":
            self.tutorial.call()
            self.dialogue_manager.start_event("end")
            self.match.player_manager.MakeDecision(
                self.game_manager.board_manager.last_decision_event.raw_event_b
            )
            self.match.match_alive = False
            return True
        return False

    def on_ron_call_avaliable(self):
        self.dialogue_manager.start_event("ron")

    def on_dialogue_event_ending(self, event_name):
        print('Stage 1')
        if event_name == "end":
            self.dialogue_manager.current_event = None
            self.match.current_board.decision_pending = False
            self.buttons["advance_text"].hide()
            self.buttons["text"].kill()
            self.game_manager.set_active_view("main_menu")
            self.game_manager.sound_manager.play_music("lobby")
            self.match.player_manager.reset(clear=True)
            self.match = None
            return
        if event_name == "skip_pon":
            print('Stage 2')
            self.dialogue_manager.start_event("discard_tip")
            self.match.current_board.decision_pending = False
        else:
            self.dialogue_manager.current_event = None
            self.match.current_board.decision_pending = False
            self.buttons["advance_text"].hide()
            self.buttons["text"].kill()
