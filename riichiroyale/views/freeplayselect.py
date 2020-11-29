import os
from pygame import surface
from libmahjong import avaliable_ais
import pygame
import pygame_gui
from riichiroyale.game import get_object
from .menuview import MenuView

VALID_AI_OPTIONS = ["AngryDiscardoBot", "Fast Tanyao", "GentlemanBot", "StubbornBot", "ThriceBot", "TotoBot"]

class FreePlaySelect(MenuView):
    def __init__(self, game_manager, screen_width, screen_height):
        bot_map = get_object('boticons')['bot']

        self.ai_lookup = dict()

        for bot in bot_map:
            if bot['ai'] in VALID_AI_OPTIONS:
                self.ai_lookup[bot['name']] = bot['ai']


        ui_manager, process_ui_event = create_menu(
            game_manager, screen_width, screen_height, self.ai_lookup
        )
        super().__init__("freeplayselect", ui_manager)
        self.screen_width = screen_width
        self.screen_height = screen_height

        self.background = surface.Surface((screen_width, screen_height)).convert_alpha()
        self.background.fill((7, 99, 36))
        self.process_ui_event = lambda x: process_ui_event(self, x)
        self.selected_bots = ["Bystander"] * 3

    def draw(self, screen):
        screen.blit(self.background, (0, 0))
        super().draw(screen)


def create_menu(game_manager, screen_width, screen_height, ai_lookup):
    current_path = os.path.dirname(os.path.realpath(__file__))
    avaliable_opponents = ai_lookup.keys()

    ui_manager = pygame_gui.UIManager(
        (screen_width, screen_height),
        os.path.join(current_path, "..", "resources", "theme.json"),
    )
    freeplay_rect = pygame.Rect(
        screen_width / 2 - 500, screen_height / 2 - 350, 1000, 700
    )
    freeplay_panel = pygame_gui.elements.UIPanel(
        relative_rect=freeplay_rect,
        starting_layer_height=1,
        manager=ui_manager,
        anchors={"top": "top", "bottom": "bottom", "left": "left", "right": "right"},
    )


    dropdowns = []

    DROPDOWN_START_RECT = (freeplay_rect.x + 30, freeplay_rect.y + 120)
    for x in range(3):
      y_offset = 120 * x
      dropdown_label_rect = pygame.Rect(0, 0, 200, 50)
      dropdown_label_rect.topleft = (0, DROPDOWN_START_RECT[1] + y_offset)
      pygame_gui.elements.UILabel(
        relative_rect=dropdown_label_rect,
        container=freeplay_panel,
        text="Bot {}".format(x + 1),
        
        manager=ui_manager,
      )

      dropdown_rect = pygame.Rect(0, 0, 200, 50)
      dropdown_rect.topleft = (DROPDOWN_START_RECT[0], DROPDOWN_START_RECT[1] + y_offset + 100)
      dropdown = pygame_gui.elements.UIDropDownMenu(
        options_list=avaliable_opponents,
        starting_option="Bystander",
        relative_rect=dropdown_rect,
        manager=ui_manager
      )
      dropdowns += [dropdown]

    back_button_rect = pygame.Rect(0, 0, 100, 50)
    back_button_rect.bottomleft = (10, -10)
    back_button = pygame_gui.elements.UIButton(
        relative_rect=back_button_rect,
        container=freeplay_panel,
        text="Back",
        manager=ui_manager,
        anchors={"top": "bottom", "bottom": "bottom", "left": "left", "right": "left"},
    )

    player_button_rect = pygame.Rect(0, 0, 100, 50)
    player_button_rect.bottomleft = (200, -10)
    play_button = pygame_gui.elements.UIButton(
        relative_rect=player_button_rect,
        container=freeplay_panel,
        text="Play",
        manager=ui_manager,
        anchors={"top": "bottom", "bottom": "bottom", "left": "left", "right": "left"}
    )

    label_rect = pygame.Rect(0, 0, 200, 100)
    label_rect.midtop = (500, 25)
    pygame_gui.elements.UILabel(
        relative_rect=label_rect,
        container=freeplay_panel,
        text="Freeplay",
        manager=ui_manager,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )


    # process_ui_event() is called when a UI event is caught while this menu is active
    def process_ui_event(view, event):
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == back_button:
                game_manager.set_active_view("main_menu")
            if event.ui_element == play_button:
                game_manager.set_active_view("game")
                game_manager.get_active_view().ai_list = ["Player"] + list(map(lambda x: ai_lookup[x], view.selected_bots))
                game_manager.get_active_view().on_match_start()
        if event.user_type == pygame_gui.UI_DROP_DOWN_MENU_CHANGED:
            for x in range(3):
                if event.ui_element == dropdowns[x]:
                    view.selected_bots[x] = event.text
                    break

    return ui_manager, process_ui_event
