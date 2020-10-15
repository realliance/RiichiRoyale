import os
import math
import random
import pygame_gui
import pygame
from pygame import Rect
from pygame import surface
import libmahjong
from riichiroyale import Player, BoardRender, Match
from .menuview import MenuView

class GameView(MenuView):
  def __init__(self, game_manager, sound_manager, screen, tile_dict, small_tile_dict, screen_width, screen_height, width_ratio, height_ratio):
    ui_manager, process_ui_event = create_game_elements(game_manager, screen_width, screen_height)
    super().__init__("game", ui_manager)
    self.tutorial = None
    self.screen = screen
    self.screen_width = screen_width
    self.screen_height = screen_height
    self.screen_width_ratio = width_ratio
    self.screen_height_ratio = height_ratio

    # Fill background
    background = surface.Surface(screen.get_size())
    self.background = background.convert_alpha()
    self.background.fill((7, 99, 36))

    self.sound_manager = sound_manager
    self.process_ui_event = process_ui_event

    # Play Area Surface
    self.player_area_rect = self.get_play_area_pos(screen)
    self.play_area = surface.Surface((self.player_area_rect.width, self.player_area_rect.height), flags=pygame.SRCALPHA)

    self.match = Match(sound_manager)
    self.match.register_player(Player("Player"))
    self.match.register_player(Player("Bot 1"))
    self.match.register_player(Player("Bot 2"))
    self.match.register_player(Player("Bot 3"))

    self.tile_dict = tile_dict
    self.small_tile_dict = small_tile_dict
    self.board_render = None

  def get_play_area_pos(self, screen):
    w, h = screen.get_size()
    play_area_height = h
    player_area_width = math.floor(h * (float(self.screen_width_ratio)/self.screen_height_ratio))
    return Rect((w - player_area_width)//2, 0, player_area_width, play_area_height)

  def on_match_start(self, tutorial_info=None):
    self.tutorial = tutorial_info
    wall = None
    deadwall = None
    if tutorial_info is not None:
      wall = tutorial_info.wall
      deadwall = tutorial_info.deadwall
    self.match.new_board(wall=wall, deadwall=deadwall)
    random.shuffle(self.sound_manager.music_playlist)
    self.sound_manager.start_playlist()
    self.board_render = BoardRender(self.small_tile_dict, self.tile_dict, self.play_area, self.match.current_board, 0)
    self.match.current_board.on_turn()

  def update(self, time_delta):
    self.board_render.update(self.tutorial)
    super().update(time_delta)

  def draw(self, screen):
    self.board_render.draw(self.background)
    screen.blit(self.background, (0, 0))
    screen.blit(self.play_area, (self.player_area_rect.x, self.player_area_rect.y))
    super().draw(screen)

def create_game_elements(game_manager, screen_width, screen_height):
  current_path = os.path.dirname(os.path.realpath(__file__))
  ui_manager = pygame_gui.UIManager((screen_width, screen_height), os.path.join(current_path, '../resources/theme.json'))

  call_menu_rect = pygame.Rect(screen_width / 2 - 500, screen_height / 2 - 350, 1000, 700)
  call_menu_panel = pygame_gui.elements.UIPanel(relative_rect=call_menu_rect,
                                                    starting_layer_height=1,
                                                    manager=ui_manager,
                                                    anchors={
                                                        'top': 'top',
                                                        'bottom': 'bottom',
                                                        'left': 'left',
                                                        'right': 'right'
                                                    })

  pon_button_rect = pygame.Rect(0, 0, 100, 50)
  pon_button_rect.bottomleft = (10, -10)
  pon_button = pygame_gui.elements.UIButton(relative_rect=pon_button_rect,
                                              container=call_menu_panel,
                                              text='Pon',
                                              manager=ui_manager,
                                              anchors={
                                                  'top': 'bottom',
                                                  'bottom': 'bottom',
                                                  'left': 'left',
                                                  'right': 'left'
                                              })

  def process_ui_event(event):
    if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
      if event.ui_element == pon_button:
        print('Pressed pon')
  
  return ui_manager, process_ui_event

