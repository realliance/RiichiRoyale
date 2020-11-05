from libmahjong import PieceType
from riichiroyale.game import Player, Match, TutorialBot, Call, DialogManager
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
        self.tutorial = None

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
        self.match = Match(None, self.game_manager.sound_manager)

    def on_pov_init(self):
        self.player = Player("Player")
        self.match.register_player(self.player)
        self.match.player_id = 0

        self.match.register_player(TutorialBot("Bot 1", self.tutorial.winning_tile))
        self.match.register_player(TutorialBot("Bot 2", self.tutorial.winning_tile))
        self.match.register_player(TutorialBot("Bot 3", self.tutorial.winning_tile))
        for p in self.match.players:
            p.hand = []
            p.melded_hand = []
            p.riichi_declared = False
            p.calls_avaliable = []
            p.hand_open = False
            p.discard_pile = []
        self.match.new_board(wall=self.tutorial.wall, deadwall=self.tutorial.deadwall)

    def on_match_start(self):
        self.pons = 0
        self.dialogue_manager.start_event("intro")
        super().on_match_start()
        self.match.current_board.on_turn()

    def on_tile_pressed(self, owner, tile_hand_index):
        tile = owner.hand[tile_hand_index]
        if owner.my_turn and self.tutorial.next_discard == tile:
            del owner.hand[tile_hand_index]
            owner.hand.sort()
            owner.discard_pile.append(tile)
            if owner.board is None:
                raise "Cannot communicate with board! Is this player registered?"
            owner.board.on_discard(owner)
            self.tutorial.discard()
            owner.my_turn = False

    def on_pon_button_pressed(self):
        if self.tutorial.next_call == "pon":
            self.tutorial.call()
            self.pons += 1
            self.dialogue_manager.start_event("after_pon")
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Pon)
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
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Chi)
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
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Skip)
            self.player.calls_avaliable = []
            return True
        return False

    def on_ron_button_pressed(self):
        if self.tutorial.next_call == "ron":
            self.tutorial.call()
            self.dialogue_manager.start_event("end")
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Ron)
            return True
        return False

    def on_ron_call_avaliable(self):
        self.dialogue_manager.start_event("ron")

    def on_dialogue_event_ending(self, event_name):
        if self.dialogue_manager.current_event == "end":
            self.game_manager.set_active_view("main_menu")
            self.game_manager.sound_manager.play_music("lobby")
        if self.dialogue_manager.current_event == "skip_pon":
            self.dialogue_manager.start_event("discard_tip")
            self.match.current_board.decision_pending = False
        else:
            self.dialogue_manager.current_event = None
            self.match.current_board.decision_pending = False
            self.buttons["advance_text"].hide()
            self.buttons["text"].kill()
