import os
from pygame import surface
import pygame
import pygame_gui
from riichiroyale.utils import settingsmanager
from .menuview import MenuView


class Settings(MenuView):
    def __init__(self, game_manager, screen_width, screen_height):
        ui_manager, process_ui_event = create_settings_menu(
            game_manager, screen_width, screen_height
        )
        super().__init__("settings", ui_manager)
        self.screen_width = screen_width
        self.screen_height = screen_height

        self.background = surface.Surface((screen_width, screen_height)).convert_alpha()
        self.background.fill((7, 99, 36))
        self.process_ui_event = process_ui_event

    def draw(self, screen):
        screen.blit(self.background, (0, 0))
        super().draw(screen)


def create_settings_menu(game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    ui_manager = pygame_gui.UIManager(
        (screen_width, screen_height),
        os.path.join(current_path, "../resources/theme.json"),
    )
    settingsmenu_rect = pygame.Rect(
        screen_width / 2 - 500, screen_height / 2 - 350, 1000, 700
    )
    settingsmenu_panel = pygame_gui.elements.UIPanel(
        relative_rect=settingsmenu_rect,
        starting_layer_height=1,
        manager=ui_manager,
        anchors={"top": "top", "bottom": "bottom", "left": "left", "right": "right"},
    )

    back_button_rect = pygame.Rect(0, 0, 100, 50)
    back_button_rect.bottomleft = (10, -10)
    back_button = pygame_gui.elements.UIButton(
        relative_rect=back_button_rect,
        container=settingsmenu_panel,
        text="Back",
        manager=ui_manager,
        anchors={"top": "bottom", "bottom": "bottom", "left": "left", "right": "left"},
    )

    settings_label_rect = pygame.Rect(0, 0, 200, 100)
    settings_label_rect.midtop = (500, 25)
    pygame_gui.elements.UILabel(
        relative_rect=settings_label_rect,
        container=settingsmenu_panel,
        text="Settings",
        manager=ui_manager,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )

    master_volume_label_rect = pygame.Rect(0, 0, 300, 100)
    master_volume_label_rect.midleft = (420, 150)
    pygame_gui.elements.UILabel(
        relative_rect=master_volume_label_rect,
        container=settingsmenu_panel,
        text="Master Volume:",
        manager=ui_manager,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )

    music_volume_label_rect = pygame.Rect(0, 0, 300, 100)
    music_volume_label_rect.midleft = (430, 250)
    pygame_gui.elements.UILabel(
        relative_rect=music_volume_label_rect,
        container=settingsmenu_panel,
        text="Music Volume:",
        manager=ui_manager,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )

    sfx_volume_label_rect = pygame.Rect(0, 0, 300, 100)
    sfx_volume_label_rect.midleft = (430, 350)
    pygame_gui.elements.UILabel(
        relative_rect=sfx_volume_label_rect,
        container=settingsmenu_panel,
        text="Effect Volume:",
        manager=ui_manager,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )

    master_volume_rect = pygame.Rect(0, 0, 200, 50)
    master_volume_rect.midleft = (700, 150)
    master_volume_slider = pygame_gui.elements.UIHorizontalSlider(
        relative_rect=master_volume_rect,
        container=settingsmenu_panel,
        manager=ui_manager,
        value_range=(0, 100),
        start_value=100,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )

    music_volume_rect = pygame.Rect(0, 0, 200, 50)
    music_volume_rect.midleft = (700, 250)
    music_volume_slider = pygame_gui.elements.UIHorizontalSlider(
        relative_rect=music_volume_rect,
        container=settingsmenu_panel,
        manager=ui_manager,
        value_range=(0, 100),
        start_value=100,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )
    sfx_volume_rect = pygame.Rect(0, 0, 200, 50)
    sfx_volume_rect.midleft = (700, 350)
    sfx_volume_slider = pygame_gui.elements.UIHorizontalSlider(
        relative_rect=sfx_volume_rect,
        container=settingsmenu_panel,
        manager=ui_manager,
        value_range=(0, 100),
        start_value=100,
        anchors={"left": "left", "right": "right", "top": "top", "bottom": "top"},
    )

    # Create settings manager to load and save settings file
    settings = settingsmanager.SettingsManager()

    # Put sliders in saved positions
    master_volume_slider.set_current_value(settings.get_setting("Master Volume"))
    sfx_volume_slider.set_current_value(settings.get_setting("SFX Volume"))
    music_volume_slider.set_current_value(settings.get_setting("Music Volume"))

    # process_ui_event() is called when a UI event is caught while this menu is active
    def process_ui_event(event):
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == back_button:
                game_manager.set_active_view("main_menu")
                settings.save_to_file()
        elif event.user_type == pygame_gui.UI_HORIZONTAL_SLIDER_MOVED:
            game_manager.sound_manager.set_music_volume(
                music_volume_slider.get_current_value() / 100.0
            )
            game_manager.sound_manager.set_sfx_volume(
                sfx_volume_slider.get_current_value() / 100.0
            )
            game_manager.sound_manager.set_master_volume(
                master_volume_slider.get_current_value() / 100.0
            )

            settings.set_setting(
                "Master Volume", master_volume_slider.get_current_value()
            )
            settings.set_setting("SFX Volume", sfx_volume_slider.get_current_value())
            settings.set_setting(
                "Music Volume", music_volume_slider.get_current_value()
            )

    return ui_manager, process_ui_event
