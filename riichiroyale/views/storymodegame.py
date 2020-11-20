import pygame
import pygame_gui
from libmahjong import start_game, EventType, EngineEvent, PieceType
from riichiroyale.game import Player, Match, process_event_queue, DialogManager, loadStory
from riichiroyale.utils import load_image_resource
from .game import GameView


class StoryModeGame(GameView):
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
    ):
        self.player_manager = player_manager
        self.ai_list = ["Player"] + ["AngryDiscardoBot"] * 3
        self.match = None
        self.match_dict = None

        super().__init__(
            game_manager,
            screen,
            tile_dict,
            small_tile_dict,
            screen_width,
            screen_height,
            width_ratio,
            height_ratio,
            player_manager=player_manager,
            name="storymodegame",
            textbox_hook=lambda uiM, bM, t: new_dialogue(self.match_dict, uiM, bM, t)
        )

    def load_match(self, match_dict):
        self.ai_list = ["Player"] + match_dict["ais"]
        self.match_dict = match_dict
        prefix = "<b>{}</b><br><br>".format(self.match_dict['opponent'])
        
        for possible_event in ["intro", "call_pon", "call_chi", "call_kan", "call_ron", "call_riichi", "on_player_wins", "on_player_loses", "on_both_lose"]:
            if possible_event in self.match_dict:
                self.dialogue_manager.register_dialog_event(possible_event)
                self.dialogue_manager.append_dialog_event(possible_event, list(map(lambda x: prefix + x, self.match_dict[possible_event])))

    def on_round_start(self):
        super().on_round_start()
        if 'intro' in self.match_dict:
            self.dialogue_manager.start_event('intro')


    def on_tile_pressed(self, owner, tile_hand_index):
        if owner.my_turn:
            tile = owner.hand[tile_hand_index]
            if (
                self.game_manager.board_manager.waiting_on_decision
                and len(owner.calls_avaliable) == 0
            ):
                self.match.play_clack()
                event = EngineEvent()
                event.type = EventType.Discard
                event.piece = int(tile.get_raw_value())
                event.player = owner.player_id
                event.decision = True
                self.player_manager.MakeDecision(event)

    def on_pon_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
        return True

    def on_chi_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
        return True

    def on_kan_button_pressed(self):
        if (
            self.game_manager.board_manager.last_decision_event.type
            == EventType.ConvertedKan
        ):
            self.match.player_manager.MakeDecision(
                self.game_manager.board_manager.last_decision_event
            )
        else:
            self.match.player_manager.MakeDecision(
                self.game_manager.board_manager.last_decision_event.raw_event_b
            )
        return True

    def on_tsumo_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
        return True

    def on_riichi_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event
        )
        return True

    def on_ron_button_pressed(self):
        self.match.player_manager.MakeDecision(
            self.game_manager.board_manager.last_decision_event.raw_event_b
        )
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

    def _end_round_dialog(self):
        self.ai_game_active = False
        self.game_manager.board_manager.round_should_end = False
        self.dialogue_manager.register_dialog_event("round_end")
        self.dialogue_manager.append_dialog_event(
            "round_end", ["Round Complete! Now for the Results..."]
        )
        i = 0
        for score in self.match.scores:
            self.dialogue_manager.append_dialog_event(
                "round_end", ["Player {0} was awarded {1} points!".format(i + 1, score)]
            )
            i += 1
        self.dialogue_manager.append_dialog_event(
            "round_end",
            [
                "Thank you for playing a demo match of Riichi Royale! Press Next to return to the main menu."
            ],
        )
        self.dialogue_manager.start_event("round_end")
        self.player.calls_avaliable = []
        for button in self.buttons:
            self.buttons[button].hide()

    def update(self, time_delta):
        if self.game_manager.board_manager.round_should_end:
            self._end_round_dialog()

        super().update(time_delta)

def new_dialogue(match_dict, gui_manager, button_map, text):
    icon_size = (125, 125)
    icon_rect = pygame.Rect(0, 0, icon_size[0], icon_size[1])
    icon_rect.bottomleft = (20, -270)
    logo_surface = pygame.Surface(icon_size, pygame.SRCALPHA)
    load_image_resource(match_dict['icon'], logo_surface, size=icon_size)
    pygame_gui.elements.ui_image.UIImage(
        relative_rect=icon_rect,
        image_surface=logo_surface,
        manager=gui_manager,
        anchors={"left": "left", "right": "left", "top": "bottom", "bottom": "bottom"},
    )

    text_box_rect = pygame.Rect(0, 0, 1366 - 40, 200)
    text_box_rect.bottomleft = (20, -70)
    text_box = pygame_gui.elements.UITextBox(
        relative_rect=text_box_rect,
        visible=False,
        html_text=text,
        manager=gui_manager,
        anchors={
            "top": "bottom",
            "bottom": "bottom",
            "left": "left",
            "right": "left",
        },
    )
    text_next_button_rect = pygame.Rect(0, 0, 100, 50)
    text_next_button_rect.bottomleft = (-120, -20)
    text_next = pygame_gui.elements.UIButton(
        relative_rect=text_next_button_rect,
        visible=False,
        text="Next",
        manager=gui_manager,
        anchors={
            "top": "bottom",
            "bottom": "bottom",
            "left": "right",
            "right": "right",
        },
    )

    button_map["text"] = text_box
    button_map["advance_text"] = text_next