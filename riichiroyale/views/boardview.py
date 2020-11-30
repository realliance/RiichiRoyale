import os
import math
import random
import atexit
import pygame_gui
import pygame
import numpy
from copy import deepcopy
from pygame import Rect
from pygame import surface
from riichiroyale import BoardRender
from riichiroyale.game import Call, get_object
from .menuview import MenuView

def clean_up_board_view(board_view):
    if board_view.match is not None:
        board_view.match.match_alive = False

class BoardView(MenuView):
    def __init__(
        self,
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
        textbox_hook=None
    ):
        ui_manager, process_ui_event = self.create_game_elements(
            screen_width, screen_height
        )
        super().__init__(name, ui_manager)
        self.screen = screen
        self.screen_width = screen_width
        self.screen_height = screen_height
        self.screen_width_ratio = width_ratio
        self.screen_height_ratio = height_ratio
        self.game_manager = game_manager
        self.match_pov = None
        self.bot_icon_cache = get_object('boticons')['bot']
        self.lock_user_input = False

        # Fill background
        background = surface.Surface(screen.get_size())
        self.background = background.convert_alpha()
        self.background.fill((7, 99, 36))

        loading_background = surface.Surface(screen.get_size())
        self.loading_background = loading_background.convert_alpha()
        self.loading_background.fill((0, 0, 0))

        self.sound_manager = self.game_manager.sound_manager
        self.dialogue_manager = dialogue_manager
        self.process_ui_event = process_ui_event

        # Play Area Surface
        self.player_area_rect = self.get_play_area_pos(screen)
        self.play_area = surface.Surface(
            (self.player_area_rect.width, self.player_area_rect.height),
            flags=pygame.SRCALPHA,
        )

        # Tile Dictionaries
        self.tile_dict = tile_dict
        self.small_tile_dict = small_tile_dict

        self.match = None
        self.board_render = None
        self.previous_player_calls_avaliable = []

        atexit.register(clean_up_board_view, self)

        if textbox_hook is not None:
            self.new_textbox = textbox_hook
        else:
            self.new_textbox = new_text_box

        self.dialogue_elements = []

    def on_match_init(self):
        """Called to initialize a new, default match. Should define self.match"""
        raise NotImplementedError()

    def on_pov_init(self):
        """Called to register the point of view for UI elements. Should define self.player"""
        raise NotImplementedError()

    def on_tile_pressed(self, owner, tile_hand_index):
        """Called when a tile is clicked on
        :param owner: Player Object. The player object that owns the tile
        :param tile_hand-index: Number. The index of the tile clicked in the owners hand.
        """
        raise NotImplementedError()

    def get_play_area_pos(self, screen):
        w, h = screen.get_size()
        play_area_height = h
        player_area_width = math.floor(
            h * (float(self.screen_width_ratio) / self.screen_height_ratio)
        )
        return Rect(
            (w - player_area_width) // 2, 0, player_area_width, play_area_height
        )

    def on_round_start(self):
        print('Round start called')
        """Called on the beginning of each round."""
        self.play_area = surface.Surface(
            (self.player_area_rect.width, self.player_area_rect.height),
            flags=pygame.SRCALPHA,
        )
        self.board_render = None
        self.board_render = BoardRender(
            self.small_tile_dict,
            self.tile_dict,
            self.play_area,
            self.match,
            self.match_pov,
            self.game_manager.board_manager
        )
        self.board_render.update()
        self.board_render.force_redraw()

    def on_match_start(self):
        """Called upon the start of a match, which comprises a series of rounds"""
        background = surface.Surface(self.screen.get_size())
        self.background = background.convert_alpha()
        self.background.fill((7, 99, 36))

        self.match_pov = None
        self.on_match_init()
        self.on_pov_init()

        random.shuffle(self.sound_manager.music_playlist)
        self.sound_manager.start_playlist()

    @property
    def player(self):
        if self.match_pov is None:
            raise "Unable to determine pov player, is match_pov set?"
        return self.match.players[self.match_pov]

    def update(self, time_delta):
        if self.match.match_ready and self.match_pov is None:
            self.match_pov = self.match.player_manager.player_id
            self.on_round_start()

        if self.match_pov is None:
            return

        if self.player.calls_avaliable != self.previous_player_calls_avaliable and not self.lock_user_input:
            self.previous_player_calls_avaliable = deepcopy(self.player.calls_avaliable)
            if len(self.player.calls_avaliable) > 0:
                self.buttons["skip"].show()
            for decision in self.player.calls_avaliable:
                if decision == Call.Chi:
                    self.on_chi_call_avaliable()
                    self.buttons["chi"].show()
                elif decision == Call.Pon:
                    self.on_pon_call_avaliable()
                    self.buttons["pon"].show()
                elif decision in (Call.Kan, Call.Concealed_Kan):
                    self.on_kan_call_avaliable()
                    self.buttons["kan"].show()
                elif decision == Call.Riichi:
                    self.on_riichi_call_avaliable()
                    self.buttons["riichi"].show()
                elif decision == Call.Ron:
                    self.on_ron_call_avaliable()
                    self.buttons["ron"].show()
                elif decision == Call.Tsumo:
                    self.on_tsumo_call_avaliable()
                    self.buttons["tsumo"].show()

        if (
            self.dialogue_manager is not None
            and self.dialogue_manager.current_event is not None
        ):
            if (
                self.dialogue_manager.get_current_page()
                != self.buttons["text"].html_text
            ):
                self.buttons["advance_text"].kill()
                self.buttons["text"].kill()
                for element in self.dialogue_elements: element.kill()
                self.new_textbox(self.manager, self.buttons, self.dialogue_elements, self.dialogue_manager.get_current_page())
            self.buttons["text"].show()
            if (
                len(self.player.calls_avaliable) != 0
                and self.dialogue_manager.is_last_page()
            ):
                self.buttons["advance_text"].hide()
            else:
                self.buttons["advance_text"].show()

        self.board_render.update(callback_handler=self.on_tile_pressed)
        super().update(time_delta)

    def draw_loading_screen(self, screen):
        """While the match is bootstrapping on the backend, draw a loading screen instead."""
        screen.blit(self.loading_background, (0, 0))

    def draw(self, screen):
        if not self.match.match_ready:
            self.draw_loading_screen(screen)
        else:
            self.board_render.draw(self.background)
            screen.blit(self.background, (0, 0))
            screen.blit(self.play_area, (self.player_area_rect.x, self.player_area_rect.y))
        super().draw(screen)

    def on_pon_button_pressed(self):
        """Called when the pon UI button is selected. The button is hidden if this function returns true.
        :result: Bool. Whether the buttons should be hidden or not.
        """
        raise NotImplementedError()

    def on_pon_call_avaliable(self):
        """Called when a Pon call is avaliable."""

    def on_chi_button_pressed(self):
        """Called when the Chi UI button is selected. The button is hidden if this function returns true.
        :result: Bool. Whether the buttons should be hidden or not.
        """
        raise NotImplementedError()

    def on_chi_call_avaliable(self):
        """Called when a Chi call is avaliable."""

    def on_kan_button_pressed(self):
        """Called when the Kan UI button is selected. The button is hidden if this function returns true.
        :result: Bool. Whether the buttons should be hidden or not.
        """
        raise NotImplementedError()

    def on_kan_call_avaliable(self):
        """Called when a Kan call is avaliable"""

    def on_riichi_button_pressed(self):
        """Called when the Riichi UI button is selected. The button is hidden if this function returns true.
        :result: Bool. Whether the buttons should be hidden or not.
        """
        raise NotImplementedError()

    def on_riichi_call_avaliable(self):
        """Called when a Riichi call is avaliable"""

    def on_ron_button_pressed(self):
        """Called when the Riichi UI button is selected. The button is hidden if this function returns true.
        :result: Bool. Whether the buttons should be hidden or not.
        """
        raise NotImplementedError()

    def on_ron_call_avaliable(self):
        """Called when a Ron call is avaliable"""

    def on_tsumo_button_pressed(self):
        """Called when the Tsumo UI button is selected. The button is hidden if this function returns true.
        :result: Bool. Whether the buttons should be hidden or not.
        """
        raise NotImplementedError()

    def on_tsumo_call_avaliable(self):
        """Called when a Tsumo call is avaliable"""

    def on_skip_button_pressed(self):
        """Called when the Skip UI button is selected. The button is hidden if this function returns true.
        :result: Bool. Whether the buttons should be hidden or not.
        """
        raise NotImplementedError()

    def on_dialogue_event_ending(self, event_name):
        pass

    def on_dialogue_next_button_pressed(self):
        if self.dialogue_manager.is_last_page():
            self.on_dialogue_event_ending(self.dialogue_manager.current_event)
            self.dialogue_manager.current_event = None
            self.buttons["advance_text"].kill()
            self.buttons["text"].kill()
            for element in self.dialogue_elements: element.kill()
        else:
            self.dialogue_manager.next_page()

    def create_game_elements(self, screen_width, screen_height):
        ui_manager = pygame_gui.UIManager(
            (screen_width, screen_height),
            os.path.join(os.path.dirname(os.path.realpath(__file__)),
            "../resources/theme.json")
        )

        call_menu_rect = pygame.Rect(
            screen_width / 2 - 500, screen_height / 2 - 350, 1000, 700
        )

        call_menu_panel = pygame_gui.elements.UIPanel(
            relative_rect=call_menu_rect,
            starting_layer_height=1,
            visible=False,
            manager=ui_manager,
            anchors={
                "top": "top",
                "bottom": "bottom",
                "left": "left",
                "right": "right",
            },
        )

        pon_button_rect = pygame.Rect(0, 0, 100, 50)
        pon_button_rect.bottomleft = (10, -60)
        pon_button = pygame_gui.elements.UIButton(
            relative_rect=pon_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Pon",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )
        chi_button_rect = pygame.Rect(0, 0, 100, 50)
        chi_button_rect.bottomleft = (110, -60)
        chi_button = pygame_gui.elements.UIButton(
            relative_rect=chi_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Chi",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )
        kan_button_rect = pygame.Rect(0, 0, 100, 50)
        kan_button_rect.bottomleft = (210, -60)
        kan_button = pygame_gui.elements.UIButton(
            relative_rect=kan_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Kan",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )
        riichi_button_rect = pygame.Rect(0, 0, 100, 50)
        riichi_button_rect.bottomleft = (310, -60)
        riichi_button = pygame_gui.elements.UIButton(
            relative_rect=riichi_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Riichi",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )
        ron_button_rect = pygame.Rect(0, 0, 100, 50)
        ron_button_rect.bottomleft = (410, -60)
        ron_button = pygame_gui.elements.UIButton(
            relative_rect=ron_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Ron",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )
        tsumo_button_rect = pygame.Rect(0, 0, 100, 50)
        tsumo_button_rect.bottomleft = (510, -60)
        tsumo_button = pygame_gui.elements.UIButton(
            relative_rect=tsumo_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Tsumo",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )
        skip_button_rect = pygame.Rect(0, 0, 100, 50)
        skip_button_rect.bottomleft = (610, -60)
        skip_button = pygame_gui.elements.UIButton(
            relative_rect=skip_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Skip",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )

        text_box_rect = pygame.Rect(0, 0, 600, 200)
        text_box_rect.bottomleft = (400, -290)
        text_box = pygame_gui.elements.UITextBox(
            relative_rect=text_box_rect,
            visible=False,
            html_text="",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )
        text_next_button_rect = pygame.Rect(0, 0, 100, 50)
        text_next_button_rect.bottomleft = (725, -190)
        text_next = pygame_gui.elements.UIButton(
            relative_rect=text_next_button_rect,
            container=call_menu_panel,
            visible=False,
            text="Next",
            manager=ui_manager,
            anchors={
                "top": "bottom",
                "bottom": "bottom",
                "left": "left",
                "right": "left",
            },
        )

        self.buttons = {
            "panel": call_menu_panel,
            "pon": pon_button,
            "chi": chi_button,
            "kan": kan_button,
            "ron": ron_button,
            "riichi": riichi_button,
            "tsumo": tsumo_button,
            "skip": skip_button,
            "text": text_box,
            "advance_text": text_next
        }

        def process_ui_event(event):
            CALL_TABLE = {
                pon_button: self.on_pon_button_pressed,
                chi_button: self.on_chi_button_pressed,
                kan_button: self.on_kan_button_pressed,
                tsumo_button: self.on_tsumo_button_pressed,
                riichi_button: self.on_riichi_button_pressed,
                ron_button: self.on_ron_button_pressed,
                skip_button: self.on_skip_button_pressed,
            }

            if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
                # Decision UI
                if event.ui_element in CALL_TABLE.keys():
                    if CALL_TABLE[event.ui_element]():
                        self.player.calls_avaliable = []
                        for button in self.buttons:
                            self.buttons[button].hide()
                        return
                # Dialogue UI
                if event.ui_element == self.buttons["advance_text"]:
                    self.on_dialogue_next_button_pressed()

        return ui_manager, process_ui_event

def new_text_box(gui_manager, button_map, _elements, text):
    text_box_rect = pygame.Rect(0, 0, 600, 200)
    text_box_rect.bottomleft = (400, -290)
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
    text_next_button_rect.bottomleft = (900, -230)
    text_next = pygame_gui.elements.UIButton(
        relative_rect=text_next_button_rect,
        visible=False,
        text="Next",
        manager=gui_manager,
        anchors={
            "top": "bottom",
            "bottom": "bottom",
            "left": "left",
            "right": "left",
        },
    )

    button_map["text"] = text_box
    button_map["advance_text"] = text_next
