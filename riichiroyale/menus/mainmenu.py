import os
from math import sin, cos
import pygame
import pygame_gui
from pygame import surface
from pygame.sprite import Group
from riichiroyale import View, generate_start_board, TileRender
from .menu import Menu

class MainMenu(View):
  def __init__(self, game_manager, dictionary, screen_width, screen_height):
    super().__init__("main_menu")
    self.menu = create_main_menu_elements(game_manager, screen_width, screen_height)
    self.manager = self.menu.manager
    self.screen_width = screen_width
    self.screen_height = screen_height
    wall, _ = generate_start_board()
    tile_hand = wall[:14]
    self.hand = list(map(lambda tile: TileRender(dictionary, tile, (0, 0)), tile_hand))
    self.hand_group = Group(self.hand)

    self.background = surface.Surface((screen_width, screen_height)).convert_alpha()
    self.background.fill((7, 99, 36))

  def process_ui_event(self, event):
    self.menu.process_ui_event(event)

  def update(self, time_delta):
    self.menu.manager.update(time_delta)
    for index, sprite in enumerate(self.hand):
      sprite.rect = ((self.screen_height / 2) * cos(index + pygame.time.get_ticks() / 5000.0) + 700,
        (self.screen_height / 2 - 100) * sin(index + pygame.time.get_ticks() / 5000.0) + 350)

  def draw(self, screen):
    screen.blit(self.background, (0, 0))
    self.hand_group.draw(screen)
    self.menu.manager.draw_ui(screen)

def create_main_menu_elements(game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    ui_manager = pygame_gui.UIManager((screen_width, screen_height), os.path.join(current_path, '../resources/theme.json'))
    menu = Menu("main_menu", ui_manager)
    newgame_button_rect = pygame.Rect(0, 0, 100, 50)
    newgame_button_rect.bottomleft = (500, -200)
    newgame_button = pygame_gui.elements.UIButton(relative_rect=newgame_button_rect,
                                                  text='New Game',
                                                  manager=menu.manager,
                                                  anchors={
                                                      'top': 'bottom',
                                                      'bottom': 'bottom',
                                                      'left': 'left',
                                                      'right': 'left',
                                                  })

    loadgame_button_rect = pygame.Rect(0, 0, 100, 50)
    loadgame_button_rect.bottomleft = (650, -200)
    loadgame_button = pygame_gui.elements.UIButton(relative_rect=loadgame_button_rect,
                                                   text='Load Game',
                                                   manager=menu.manager,
                                                   anchors={
                                                       'top': 'bottom',
                                                       'bottom': 'bottom',
                                                       'left': 'left',
                                                       'right': 'left',
                                                   })

    settings_button_rect = pygame.Rect(0, 0, 100, 50)
    settings_button_rect.bottomleft = (800, -200)
    settings_button = pygame_gui.elements.UIButton(relative_rect=settings_button_rect,
                                                   text='Settings',
                                                   manager=menu.manager,
                                                   anchors={
                                                       'top': 'bottom',
                                                       'bottom': 'bottom',
                                                       'left': 'left',
                                                       'right': 'left',
                                                   })

    title_rect = pygame.Rect(0, 0, 800, 300)
    title_rect.center = (screen_width / 2, screen_height / 2)
    title_text = pygame_gui.elements.UILabel(relative_rect=title_rect,
                                             text='Riichi Royale',
                                             manager=menu.manager,
                                             object_id='mainMenuText',
                                             anchors={
                                                 'left': 'left',
                                                 'right': 'right',
                                                 'top': 'top',
                                                 'bottom': 'bottom'
                                             })

    menu.uiElements.append(newgame_button)
    menu.uiElements.append(loadgame_button)
    menu.uiElements.append(settings_button)
    menu.uiElements.append(title_text)

    # processuievent() is called when a UI event is caught while this menu is active
    def process_ui_event(event):
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == newgame_button:
                print('Pressed new game')
                game_manager.set_active_view('game')
                game_manager.get_active_view().on_match_start()
            if event.ui_element == settings_button:
                print('Switching to settings menu')
                game_manager.set_active_view('settings')

    menu.process_ui_event = process_ui_event
    return menu