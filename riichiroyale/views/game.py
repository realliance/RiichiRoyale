import os
import math
import random
import pygame_gui
import pygame
from pygame import Rect
from pygame import surface
import libmahjong
from riichiroyale import BoardRender, Tile
from riichiroyale.game import Player, Match, TutorialBot, Call
from .menuview import MenuView

class GameView(MenuView):
  def __init__(self, game_manager, sound_manager, screen, tile_dict, small_tile_dict, screen_width, screen_height, width_ratio, height_ratio, ai_managed=False, name="game"):
    ui_manager, process_ui_event, self.buttons = self.create_game_elements(game_manager, screen_width, screen_height)
    super().__init__(name, ui_manager)
    self.tutorial = None
    self.screen = screen
    self.screen_width = screen_width
    self.screen_height = screen_height
    self.screen_width_ratio = width_ratio
    self.screen_height_ratio = height_ratio
    self.ai_managed = ai_managed

    # Fill background
    background = surface.Surface(screen.get_size())
    self.background = background.convert_alpha()
    self.background.fill((7, 99, 36))

    self.sound_manager = sound_manager
    self.process_ui_event = process_ui_event

    # Play Area Surface
    self.player_area_rect = self.get_play_area_pos(screen)
    self.play_area = surface.Surface((self.player_area_rect.width, self.player_area_rect.height), flags=pygame.SRCALPHA)

    self.match = Match(None, sound_manager=sound_manager, ai_managed=ai_managed)

    self.tile_dict = tile_dict
    self.small_tile_dict = small_tile_dict
    self.board_render = None

  def get_play_area_pos(self, screen):
    w, h = screen.get_size()
    play_area_height = h
    player_area_width = math.floor(h * (float(self.screen_width_ratio)/self.screen_height_ratio))
    return Rect((w - player_area_width)//2, 0, player_area_width, play_area_height)

  def on_match_start(self, tutorial_info=None, dialogue_manager=None):
    self.tutorial = tutorial_info
    self.pons = 0 #counting for tutorial
    self.dialogue_manager = dialogue_manager
    if dialogue_manager is not None:
      self.dialogue_manager.start_event("intro")

    self.player = Player("Player")
    self.match.register_player(self.player)
    wall = None
    deadwall = None
    if tutorial_info is not None:
      wall = tutorial_info.wall
      deadwall = tutorial_info.deadwall
      self.match.register_player(TutorialBot("Bot 1", self.tutorial.winning_tile))
      self.match.register_player(TutorialBot("Bot 2", self.tutorial.winning_tile))
      self.match.register_player(TutorialBot("Bot 3", self.tutorial.winning_tile))
    elif self.ai_managed:
      self.match.register_player(Player("Bot 1"))
      self.match.register_player(Player("Bot 2"))
      self.match.register_player(Player("Bot 3"))
      wall = [Tile.ERROR_PIECE] * 70
      deadwall = [Tile.ERROR_PIECE] * 14
    self.match.new_board(wall=wall, deadwall=deadwall)
    random.shuffle(self.sound_manager.music_playlist)
    self.sound_manager.start_playlist()
    self.board_render = BoardRender(self.small_tile_dict, self.tile_dict, self.play_area, self.match.current_board, 0)
    if not self.ai_managed:
      self.match.current_board.on_turn()

  def update(self, time_delta):
    
    if self.player.calls_avaliable:
      self.buttons["skip"].show()
      #self.buttons["text"].show()
      for decision in self.player.calls_avaliable:
        if decision == Call.Chi:
          self.buttons["chi"].show()
          if self.dialogue_manager is not None and self.dialogue_manager.current_event is None:
            self.dialogue_manager.start_event('chi')
        elif decision == Call.Pon:
          self.buttons["pon"].show()
          if self.dialogue_manager is not None and self.dialogue_manager.current_event is None:
            if self.pons == 0:
              self.dialogue_manager.start_event('pon')
            elif self.pons == 1:
              self.dialogue_manager.start_event('skip_pon')
        elif decision == Call.Kan or decision == Call.Concealed_Kan:
          self.buttons["Kan"].show()
        elif decision == Call.Riichi:
          self.buttons["riichi"].show()
        elif decision == Call.Ron:
          self.buttons["ron"].show()
          if self.dialogue_manager is not None and self.dialogue_manager.current_event is None:
            self.dialogue_manager.start_event('ron')
        elif decision == Call.Tsumo:
          self.buttons["tsumo"].show()
      
    if self.dialogue_manager is not None and self.dialogue_manager.current_event is not None:
      if self.dialogue_manager.get_current_page() != self.buttons["text"].html_text:
        self.buttons["text"].kill()
        self.new_text_box(self.dialogue_manager.get_current_page())
      self.match.current_board.decision_pending = True
      self.buttons["text"].show()
      if len(self.player.calls_avaliable) != 0 and self.dialogue_manager.is_last_page():
        self.buttons['advance_text'].hide()
      else:
        self.buttons["advance_text"].show()

    self.board_render.update(self.tutorial)
    super().update(time_delta)

  def draw(self, screen):
    self.board_render.draw(self.background)
    screen.blit(self.background, (0, 0))
    screen.blit(self.play_area, (self.player_area_rect.x, self.player_area_rect.y))
    super().draw(screen)


  def create_game_elements(self, game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    ui_manager = pygame_gui.UIManager((screen_width, screen_height), os.path.join(current_path, '../resources/theme.json'))

    call_menu_rect = pygame.Rect(screen_width / 2 - 500, screen_height / 2 - 350, 1000, 700)
    call_menu_panel = pygame_gui.elements.UIPanel(relative_rect=call_menu_rect,
                                                      starting_layer_height=1,
                                                      visible=False,
                                                      manager=ui_manager,
                                                      anchors={
                                                          'top': 'top',
                                                          'bottom': 'bottom',
                                                          'left': 'left',
                                                          'right': 'right'
                                                      })

    pon_button_rect = pygame.Rect(0, 0, 100, 50)
    pon_button_rect.bottomleft = (10, -60)
    pon_button = pygame_gui.elements.UIButton(relative_rect=pon_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Pon',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    chi_button_rect = pygame.Rect(0, 0, 100, 50)
    chi_button_rect.bottomleft = (110, -60)
    chi_button = pygame_gui.elements.UIButton(relative_rect=chi_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Chi',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    kan_button_rect = pygame.Rect(0, 0, 100, 50)
    kan_button_rect.bottomleft = (210, -60)
    kan_button = pygame_gui.elements.UIButton(relative_rect=kan_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Kan',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    riichi_button_rect = pygame.Rect(0, 0, 100, 50)
    riichi_button_rect.bottomleft = (310, -60)
    riichi_button = pygame_gui.elements.UIButton(relative_rect=riichi_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Riichi',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    ron_button_rect = pygame.Rect(0, 0, 100, 50)
    ron_button_rect.bottomleft = (410, -60)
    ron_button = pygame_gui.elements.UIButton(relative_rect=ron_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Ron',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    tsumo_button_rect = pygame.Rect(0, 0, 100, 50)
    tsumo_button_rect.bottomleft = (510, -60)
    tsumo_button = pygame_gui.elements.UIButton(relative_rect=tsumo_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Tsumo',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    skip_button_rect = pygame.Rect(0, 0, 100, 50)
    skip_button_rect.bottomleft = (610, -60)
    skip_button = pygame_gui.elements.UIButton(relative_rect=skip_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Skip',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })

    text_box_rect = pygame.Rect(0, 0, 600, 200)
    text_box_rect.bottomleft = (400, -290)
    text_box = pygame_gui.elements.UITextBox(relative_rect=text_box_rect,
                                                visible=False,
                                                html_text="",
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    text_next_button_rect = pygame.Rect(0, 0, 100, 50)
    text_next_button_rect.bottomleft = (725, -190)
    text_next = pygame_gui.elements.UIButton(relative_rect=text_next_button_rect,
                                                container=call_menu_panel,
                                                visible=False,
                                                text='Next',
                                                manager=ui_manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    
    buttons = {
      "panel": call_menu_panel,
      "pon": pon_button,
      "chi": chi_button,
      "kan": kan_button,
      "riichi": riichi_button,
      "ron": ron_button,
      "tsumo": tsumo_button,
      "skip": skip_button,
      "text": text_box,
      "advance_text": text_next
    }

    def process_ui_event(event):
      if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
        if event.ui_element == pon_button:
          if (self.tutorial is None) or (self.tutorial.next_call == 'pon'):
            if self.tutorial is not None:
              self.tutorial.call()
              self.pons += 1
              self.dialogue_manager.start_event('after_pon')
            print('Pressed pon')
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Pon)
            for button in self.buttons:
              buttons[button].hide()
        if event.ui_element == chi_button:
          if (self.tutorial is None) or (self.tutorial.next_call == 'chi'):
            if self.tutorial is not None:
              self.tutorial.call()
              self.dialogue_manager.start_event('after_chi')
            print('Pressed chi')
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Chi)
            for button in self.buttons:
              buttons[button].hide()
        if event.ui_element == ron_button:
          if (self.tutorial is None) or (self.tutorial.next_call == 'ron'):
            if self.tutorial is not None:
              self.tutorial.call()
              self.dialogue_manager.start_event('end')
            print('Pressed ron')
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Ron)
            for button in self.buttons:
              buttons[button].hide()
        if event.ui_element == skip_button:
          if (self.tutorial is None) or (self.tutorial.next_call == 'skip'):
            self.tutorial.call()
            print('Pressed skip')
            self.match.current_board.decision_pending = False
            self.player.make_decision(Call.Skip)
            for button in self.buttons:
              buttons[button].hide()
        if event.ui_element == text_next:
          if not self.dialogue_manager.is_last_page():
            self.dialogue_manager.next_page()
            #self.match.current_board.decision_pending = False
          else:
            if self.dialogue_manager.current_event == 'end':
              game_manager.set_active_view('main_menu')
            if self.dialogue_manager.current_event == 'skip_pon':
              self.dialogue_manager.start_event('discard_tip')
              self.match.current_board.decision_pending = False
            else:
              self.dialogue_manager.current_event = None
              self.match.current_board.decision_pending = False
              text_next.hide()
              self.buttons["text"].kill()

    
    return ui_manager, process_ui_event, buttons
  
  def new_text_box(self, text):
    text_box_rect = pygame.Rect(0, 0, 600, 200)
    text_box_rect.bottomleft = (400, -290)
    text_box = pygame_gui.elements.UITextBox(relative_rect=text_box_rect,
                                                visible=False,
                                                html_text=text,
                                                manager=self.manager,
                                                anchors={
                                                    'top': 'bottom',
                                                    'bottom': 'bottom',
                                                    'left': 'left',
                                                    'right': 'left'
                                                })
    self.buttons["text"] = text_box
