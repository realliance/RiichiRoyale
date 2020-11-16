import os
from math import sin, cos, pi, floor, sqrt
import pygame
import pygame_gui
from pygame import surface
from pygame.sprite import LayeredUpdates, Sprite
from libmahjong import Piece
from riichiroyale.sprites import TileRender
from riichiroyale.game import generate_start_board
from riichiroyale.utils import load_image_resource
from .menuview import MenuView


class MainMenu(MenuView):
    def __init__(self, game_manager, dictionary, screen_width, screen_height):
        ui_manager, process_ui_event = create_main_menu_elements(
            game_manager, screen_width, screen_height
        )
        super().__init__("main_menu", ui_manager)
        self.screen_width = screen_width
        self.screen_height = screen_height
        wall, _ = generate_start_board()
        tile_hand = wall[:14]
        self.hand = list(
            map(lambda tile: TileRender(dictionary, Piece(tile), (0, 0)), tile_hand)
        )
        self.hand_group = LayeredUpdates(self.hand)

        self.background = surface.Surface((screen_width, screen_height)).convert_alpha()
        self.background.fill((7, 99, 36))
        self.process_ui_event = process_ui_event


        self.radii = ((self.screen_height / 2.8), (self.screen_height / 2.8))
        self.angle = pi/3
        self.center = (screen_width * 0.65, screen_height / 2)

        trophy_size = (floor(self.screen_height / 1.5), floor(self.screen_height / 1.5))
        trophy = surface.Surface(trophy_size, pygame.SRCALPHA)
        load_image_resource('trophy.png', trophy, size=trophy_size)
        trophy_sprite = Sprite()
        trophy_sprite.image = trophy

        trophy_pos = pygame.Rect((0, 0), trophy_size)
        trophy_pos.center = (self.center[0] + 50, self.center[1])

        trophy_sprite.rect = trophy_pos

        self.hand_group.add(trophy_sprite, layer=1)

    def update(self, time_delta):
        super().update(time_delta)
        for index, sprite in enumerate(self.hand):
            major_radius = self.radii[0]
            minor_radius = self.radii[1]
            tick_value = (index * ((2 * pi) / len(self.hand))) + pygame.time.get_ticks() / 5000.0
            angle = self.angle
            center = self.center
            x = major_radius * cos(tick_value) * cos(angle) - minor_radius * sin(tick_value) * sin(angle) + center[0]
            y = major_radius * cos(tick_value) * sin(angle) - minor_radius * sin(tick_value) * cos(angle) + center[1]
            sprite.rect = (x, y)
            overflow_tick = tick_value % (2 * pi)
            new_layer = 0 if overflow_tick > pi * 0.8 else 2
            self.hand_group.change_layer(sprite, new_layer)

    def draw(self, screen):
        screen.blit(self.background, (0, 0))
        self.hand_group.draw(screen)
        super().draw(screen)


def create_main_menu_elements(game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    ui_manager = pygame_gui.UIManager(
        (screen_width, screen_height),
        os.path.join(current_path, "../resources/theme.json"),
    )

    global_topleft = (10, 50)

    # Logo
    logo_size = (500, 250)
    logo_rect = pygame.Rect(0, 0, logo_size[0], logo_size[1])
    logo_rect.topleft = global_topleft
    logo_surface = pygame.Surface(logo_size, pygame.SRCALPHA)
    load_image_resource('riichiroyalelogov1.png', logo_surface, size=logo_size)
    pygame_gui.elements.ui_image.UIImage(
        relative_rect=logo_rect,
        image_surface=logo_surface,
        manager=ui_manager,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "bottom"},
    )

    # Story Mode
    menu_button_size = (350, 60)
    storymode_rect = pygame.Rect(0, 0, menu_button_size[0], menu_button_size[1])
    storymode_rect.topleft = (global_topleft[0], global_topleft[1] + logo_size[1] + 50)
    storymode_button = pygame_gui.elements.UIButton(
        relative_rect=storymode_rect,
        text="Story Mode",
        manager=ui_manager,
        object_id="menuButton",
        anchors={
            "top": "top",
            "bottom": "bottom",
            "left": "left",
            "right": "left",
        },
    )

    # New Game
    newgame_button_rect = pygame.Rect(0, 0, menu_button_size[0], menu_button_size[1])
    newgame_button_rect.topleft = (global_topleft[0], global_topleft[1] + logo_size[1] + 1 * (menu_button_size[1] + 30) + 50)
    newgame_button = pygame_gui.elements.UIButton(
        relative_rect=newgame_button_rect,
        text="Freeplay",
        manager=ui_manager,
        object_id="menuButton",
        anchors={
            "top": "top",
            "bottom": "bottom",
            "left": "left",
            "right": "left",
        },
    )

    # Play Tutorial
    tutorial_button_rect = pygame.Rect(0, 0, menu_button_size[0], menu_button_size[1])
    tutorial_button_rect.topleft = (global_topleft[0], global_topleft[1] + logo_size[1] + 2 * (menu_button_size[1] + 30) + 50)
    tutorial_button = pygame_gui.elements.UIButton(
        relative_rect=tutorial_button_rect,
        text="Start Tutorial",
        manager=ui_manager,
        object_id="menuButton",
        anchors={
            "top": "top",
            "bottom": "bottom",
            "left": "left",
            "right": "left",
        },
    )

    # Settings
    settings_button_rect = pygame.Rect(0, 0, menu_button_size[0], menu_button_size[1])
    settings_button_rect.topleft = (global_topleft[0], global_topleft[1] + logo_size[1] + 3 * (menu_button_size[1] + 30) + 50)
    settings_button = pygame_gui.elements.UIButton(
        relative_rect=settings_button_rect,
        text="Settings",
        manager=ui_manager,
        object_id="menuButton",
        anchors={
            "top": "top",
            "bottom": "bottom",
            "left": "left",
            "right": "left",
        },
    )

    # processuievent() is called when a UI event is caught while this menu is active
    def process_ui_event(event):
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == storymode_button:
                game_manager.set_active_view("storymodeselect")
            if event.ui_element == newgame_button:
                game_manager.set_active_view("freeplayselect")
            if event.ui_element == tutorial_button:
                game_manager.set_active_view("tutorial")
                game_manager.get_active_view().on_match_start()
            if event.ui_element == settings_button:
                game_manager.set_active_view("settings")

    return ui_manager, process_ui_event
