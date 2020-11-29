import os
from pygame import surface
from libmahjong import avaliable_ais
import pygame
import pygame_gui
from riichiroyale.game.storagemanager import get_object
from .menuview import MenuView


class StoryModeSelect(MenuView):
    def __init__(self, game_manager, screen_width, screen_height):
        ui_manager, process_ui_event, stage_buttons = create_menu(
            game_manager, screen_width, screen_height
        )
        super().__init__("storymodeselect", ui_manager)
        self.screen_width = screen_width
        self.screen_height = screen_height

        self.background = surface.Surface((screen_width, screen_height)).convert_alpha()
        self.background.fill((7, 99, 36))
        self.process_ui_event = lambda x: process_ui_event(self, x)
        self.matches = get_object('matches')['matches']
        self.stage_buttons = stage_buttons

    def on_view_enter(self):
        self.matches = get_object('matches')['matches']

    def draw(self, screen):
        screen.blit(self.background, (0, 0))
        super().draw(screen)

    def _determine_stage_button_state(self, i, matches, stage_buttons, current_stage_found = False):
        if (i >= len(matches)):
            return
        if not current_stage_found:
            if not matches[i]['completed']:
                current_stage_found = True
                stage_buttons[i].enable()
        else:
            stage_buttons[i].disable()

        self._determine_stage_button_state(i + 1, matches, stage_buttons, current_stage_found)

    def update(self, time_delta):
        self._determine_stage_button_state(0, self.matches, self.stage_buttons)
        super().update(time_delta)


def create_menu(game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    matches = get_object('matches')['matches']

    ui_manager = pygame_gui.UIManager(
        (screen_width, screen_height),
        os.path.join(current_path, "..", "resources", "theme.json"),
    )
    storymode_rect = pygame.Rect(
        screen_width / 2 - 500, screen_height / 2 - 350, 1000, 700
    )
    storymode_panel = pygame_gui.elements.UIPanel(
        relative_rect=storymode_rect,
        starting_layer_height=1,
        manager=ui_manager,
        anchors={"top": "top", "bottom": "bottom", "left": "left", "right": "right"},
    )

    stage_buttons = []

    TOPLEFT_CORNER_X = (storymode_rect.width - (275 * 3)) / 2
    TOPRIGHT_CORNER_Y = (storymode_rect.height - (275 * 2)) / 2
    x, y = (TOPLEFT_CORNER_X, TOPRIGHT_CORNER_Y)
    xi, yi = (0, 0)
    xoffset, yoffset = (275, 275)
    for match in matches:
        match_square_rect = pygame.Rect(x + (xi % 3) * xoffset, y + yi * yoffset, 250, 250)
        panel = pygame_gui.elements.UIPanel(
            relative_rect=match_square_rect,
            starting_layer_height=1,
            container=storymode_panel,
            manager=ui_manager,
            anchors={"top": "top", "bottom": "bottom", "left": "left", "right": "right"},
        )

        if 'top_line_title' in match and 'bottom_line_title' in match:
            label_rect = pygame.Rect(0, 0, 190, 100)
            label_rect.centerx = 95 + 15
            pygame_gui.elements.UILabel(
                relative_rect=label_rect,
                container=panel,
                object_id='story_match_label',
                text=match['top_line_title'],
                manager=ui_manager,
                anchors={"left": "left", "right": "right", "top": "top", "bottom": "bottom"},
            )

            label_rect = pygame.Rect(0, 0, 190, 150)
            label_rect.centerx = 95 + 15
            pygame_gui.elements.UILabel(
                relative_rect=label_rect,
                container=panel,
                object_id='story_match_label',
                text=match['bottom_line_title'],
                manager=ui_manager,
                anchors={"left": "left", "right": "right", "top": "top", "bottom": "bottom"},
            )
        else:
            label_rect = pygame.Rect(0, 0, 190, 100)
            label_rect.centerx = 95 + 15
            pygame_gui.elements.UILabel(
                relative_rect=label_rect,
                container=panel,
                object_id='story_match_label',
                text=match['opponent'],
                manager=ui_manager,
                anchors={"left": "left", "right": "right", "top": "top", "bottom": "bottom"},
            )

        player_button_rect = pygame.Rect(0, 0, 190, 50)
        player_button_rect.centerx = 95 + 15
        player_button_rect.y = 250 - 100
        play_button = pygame_gui.elements.UIButton(
            relative_rect=player_button_rect,
            container=panel,
            text="Play",
            manager=ui_manager,
            anchors={"top": "top", "bottom": "bottom", "left": "left", "right": "right"}
        )
        
        xi += 1
        yi = xi // 3

        stage_buttons += [play_button]


    back_button_rect = pygame.Rect(0, 0, 100, 50)
    back_button_rect.bottomleft = (10, -10)
    back_button = pygame_gui.elements.UIButton(
        relative_rect=back_button_rect,
        container=storymode_panel,
        text="Back",
        manager=ui_manager,
        anchors={"top": "bottom", "bottom": "bottom", "left": "left", "right": "left"},
    )

    label_rect = pygame.Rect(0, 0, 200, 100)
    label_rect.midtop = (500, 10)
    pygame_gui.elements.UILabel(
        relative_rect=label_rect,
        container=storymode_panel,
        text="Story Matches",
        manager=ui_manager,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )


    # process_ui_event() is called when a UI event is caught while this menu is active
    def process_ui_event(view, event):
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == back_button:
                game_manager.set_active_view("main_menu")
            i = 0
            for button in stage_buttons:
                if event.ui_element == button:
                    game_manager.set_active_view("storymodegame")
                    game_manager.get_active_view().load_match(matches[i], i)
                    game_manager.get_active_view().on_match_start()
                    break
                i += 1


    return ui_manager, process_ui_event, stage_buttons
