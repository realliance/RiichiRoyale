import os
from math import sin, cos
import pygame
import pygame_gui
from pygame import surface
from pygame.sprite import Group
from riichiroyale.sprites import TileRender
from riichiroyale.game import generate_start_board, Tile, DialogManager
from .menuview import MenuView

class MainMenu(MenuView):
  def __init__(self, game_manager, dictionary, screen_width, screen_height):
    ui_manager, process_ui_event = create_main_menu_elements(game_manager, screen_width, screen_height)
    super().__init__("main_menu", ui_manager)
    self.screen_width = screen_width
    self.screen_height = screen_height
    wall, _ = generate_start_board()
    tile_hand = wall[:14]
    self.hand = list(map(lambda tile: TileRender(dictionary, tile, (0, 0)), tile_hand))
    self.hand_group = Group(self.hand)

    self.background = surface.Surface((screen_width, screen_height)).convert_alpha()
    self.background.fill((7, 99, 36))
    self.process_ui_event = process_ui_event

  def update(self, time_delta):
    super().update(time_delta)
    for index, sprite in enumerate(self.hand):
      sprite.rect = ((self.screen_height / 2) * cos(index + pygame.time.get_ticks() / 5000.0) + 700,
        (self.screen_height / 2 - 100) * sin(index + pygame.time.get_ticks() / 5000.0) + 350)

  def draw(self, screen):
    screen.blit(self.background, (0, 0))
    self.hand_group.draw(screen)
    super().draw(screen)

def create_main_menu_elements(game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    ui_manager = pygame_gui.UIManager((screen_width, screen_height), os.path.join(current_path, '../resources/theme.json'))
    newgame_button_rect = pygame.Rect(0, 0, 100, 50)
    newgame_button_rect.bottomleft = (400, -200)
    newgame_button = pygame_gui.elements.UIButton(relative_rect=newgame_button_rect,
                                                  text='Start Demo',
                                                  manager=ui_manager,
                                                  anchors={
                                                      'top': 'bottom',
                                                      'bottom': 'bottom',
                                                      'left': 'left',
                                                      'right': 'left',
                                                  })

    tutorial_button_rect = pygame.Rect(0, 0, 120, 50)
    tutorial_button_rect.bottomleft = (540, -200)
    tutorial_button = pygame_gui.elements.UIButton(relative_rect=tutorial_button_rect,
                                                  text='Start Tutorial',
                                                  manager=ui_manager,
                                                  anchors={
                                                      'top': 'bottom',
                                                      'bottom': 'bottom',
                                                      'left': 'left',
                                                      'right': 'left',
                                                  })

    settings_button_rect = pygame.Rect(0, 0, 100, 50)
    settings_button_rect.bottomleft = (700, -200)
    settings_button = pygame_gui.elements.UIButton(relative_rect=settings_button_rect,
                                                   text='Settings',
                                                   manager=ui_manager,
                                                   anchors={
                                                       'top': 'bottom',
                                                       'bottom': 'bottom',
                                                       'left': 'left',
                                                       'right': 'left',
                                                   })

    title_rect = pygame.Rect(0, 0, 800, 300)
    title_rect.center = (screen_width / 2, screen_height / 2)
    pygame_gui.elements.UILabel(relative_rect=title_rect,
                                             text='Riichi Royale',
                                             manager=ui_manager,
                                             object_id='mainMenuText',
                                             anchors={
                                                 'left': 'left',
                                                 'right': 'right',
                                                 'top': 'top',
                                                 'bottom': 'bottom'
                                             })


    load_button_rect = pygame.Rect(0, 0, 100, 50)
    load_button_rect.bottomleft = (850, -200)
    load_button = pygame_gui.elements.UIButton(relative_rect=load_button_rect,
                                               text='Load Game',
                                               manager=ui_manager,
                                               anchors={
                                                   'top': 'bottom',
                                                   'bottom': 'bottom',
                                                   'left': 'left',
                                                   'right': 'left',
                                               })

    load_dialog_shown = False

    # processuievent() is called when a UI event is caught while this menu is active
    def process_ui_event(event):
        nonlocal load_dialog_shown
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == newgame_button:
                game_manager.set_active_view('game')
                game_manager.get_active_view().on_match_start()
            if event.ui_element == tutorial_button:
                game_manager.set_active_view('tutorial')
                game_manager.get_active_view().on_match_start()
            if event.ui_element == settings_button:
                game_manager.set_active_view('settings')
            if event.ui_element == load_button:
                if not load_dialog_shown:
                    load_file_rect = pygame.Rect(0, 0, 500, 300)
                    load_file_rect.bottomleft = (600, 300)
                    load_file_dialog = pygame_gui.windows.UIFileDialog(rect=load_file_rect,
                                                                       manager=ui_manager,
                                                                       window_title='Select file to load.',
                                                                       visible=False,
                                                                       allow_existing_files_only=False)
                    print('Showing load dialog.')
                    load_file_dialog.show()
                    load_dialog_shown = True
        if event.user_type == pygame_gui.UI_FILE_DIALOG_PATH_PICKED:
            print('Path to load:', event.text)
        if event.user_type == pygame_gui.UI_WINDOW_CLOSE:
            if event.ui_object_id == '#file_dialog':
                print('Load dialog closed.')
                load_dialog_shown = False
                

    return ui_manager, process_ui_event