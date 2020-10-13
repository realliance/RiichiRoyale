import os
import pygame
import pygame_gui
from riichiroyale import View

class GameManager:
  def __init__(self):
    self.views = dict()
    self.current_view = None

  def add_view(self, view):
    self.views[view.name] = view

  def process_ui_event(self, event):
    self.views[self.current_view].process_ui_event(event)

  def update_gui_manager(self, event):
    if self.views[self.current_view].manager is not None:
      self.views[self.current_view].manager.process_events(event)

  def set_active_view(self, name):
    self.current_view = name

  def update(self, time_delta):
    self.views[self.current_view].update(time_delta)

  def draw(self, screen):
    self.views[self.current_view].draw(screen)

class Menu(View):
  def __init__(self, name, gui_manager, elements=None):
    super().__init__(name)
    self.manager = gui_manager
    if elements is None:
      self.uiElements = []
    else:
      self.uiElements = elements

    self.process_ui_event = None

  def update(self, time_delta):
    return self.manager.update(time_delta)

  def draw(self, screen):
    self.manager.draw_ui(screen)

def create_main_menu(game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    menu = Menu("main_menu", pygame_gui.UIManager((screen_width, screen_height), os.path.join(current_path, '../resources/theme.json')))
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
            if event.ui_element == settings_button:
                print('Switching to settings menu')
                game_manager.set_active_view('settings')

    menu.process_ui_event = process_ui_event
    return menu


def create_settings_menu(game_manager, screen_width, screen_height):
    current_path = os.path.dirname(os.path.realpath(__file__))
    menu = Menu('settings', pygame_gui.UIManager((screen_width, screen_height), os.path.join(current_path, '../resources/theme.json')))
    settingsmenu_rect = pygame.Rect(screen_width / 2 - 500, screen_height / 2 - 350, 1000, 700)
    settingsmenu_panel = pygame_gui.elements.UIPanel(relative_rect=settingsmenu_rect,
                                                     starting_layer_height=1,
                                                     manager=menu.manager,
                                                     anchors={
                                                         'top': 'top',
                                                         'bottom': 'bottom',
                                                         'left': 'left',
                                                         'right': 'right'
                                                     })

    back_button_rect = pygame.Rect(0, 0, 100, 50)
    back_button_rect.bottomleft = (10, -10)
    back_button = pygame_gui.elements.UIButton(relative_rect=back_button_rect,
                                               container=settingsmenu_panel,
                                               text='Back',
                                               manager=menu.manager,
                                               anchors={
                                                   'top': 'bottom',
                                                   'bottom': 'bottom',
                                                   'left': 'left',
                                                   'right': 'left'
                                               })

    settings_label_rect = pygame.Rect(0, 0, 200, 100)
    settings_label_rect.midtop = (500, 25)
    settings_label = pygame_gui.elements.UILabel(relative_rect=settings_label_rect,
                                                 container=settingsmenu_panel,
                                                 text='Settings',
                                                 manager=menu.manager,
                                                 anchors={
                                                     'left': 'left',
                                                     'right': 'right',
                                                     'top': 'top',
                                                     'bottom': 'top'
                                                 })

    volume_label_rect = pygame.Rect(0, 0, 200, 100)
    volume_label_rect.midleft = (500, 150)
    volume_label = pygame_gui.elements.UILabel(relative_rect=volume_label_rect,
                                               container=settingsmenu_panel,
                                               text='Volume:',
                                               manager=menu.manager,
                                               anchors={
                                                   'left': 'left',
                                                   'right': 'right',
                                                   'top': 'top',
                                                   'bottom': 'top'
                                               })

    master_volume_rect = pygame.Rect(0, 0, 200, 50)
    master_volume_rect.midleft = (700, 150)
    master_volume_slider = pygame_gui.elements.UIHorizontalSlider(relative_rect=master_volume_rect,
                                                                  container=settingsmenu_panel,
                                                                  manager=menu.manager,
                                                                  value_range=(0, 100),
                                                                  start_value=100,
                                                                  anchors={
                                                                      'left': 'left',
                                                                      'right': 'right',
                                                                      'top': 'top',
                                                                      'bottom': 'top'
                                                                  })

    sfx_volume_rect = pygame.Rect(0, 0, 200, 50)
    sfx_volume_rect.midleft = (700, 250)
    sfx_volume_slider = pygame_gui.elements.UIHorizontalSlider(relative_rect=sfx_volume_rect,
                                                                  container=settingsmenu_panel,
                                                                  manager=menu.manager,
                                                                  value_range=(0, 100),
                                                                  start_value=100,
                                                                  anchors={
                                                                      'left': 'left',
                                                                      'right': 'right',
                                                                      'top': 'top',
                                                                      'bottom': 'top'
                                                                  })

    music_volume_rect = pygame.Rect(0, 0, 200, 50)
    music_volume_rect.midleft = (700, 250)
    music_volume_slider = pygame_gui.elements.UIHorizontalSlider(relative_rect=music_volume_rect,
                                                               container=settingsmenu_panel,
                                                               manager=menu.manager,
                                                               value_range=(0, 100),
                                                               start_value=100,
                                                               anchors={
                                                                   'left': 'left',
                                                                   'right': 'right',
                                                                   'top': 'top',
                                                                   'bottom': 'top'
                                                               })

    menu.uiElements.append(settingsmenu_panel)
    menu.uiElements.append(back_button)
    menu.uiElements.append(settings_label)
    menu.uiElements.append(master_volume_slider)
    menu.uiElements.append(sfx_volume_slider)
    menu.uiElements.append(music_volume_slider)
    menu.uiElements.append(volume_label)

    # process_ui_event() is called when a UI event is caught while this menu is active
    def process_ui_event(event):
      if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
          if event.ui_element == back_button:
              print('Pressed back')
              game_manager.set_active_view('main_menu')
      elif event.user_type == pygame_gui.UI_HORIZONTAL_SLIDER_MOVED:
        #soundWrapper.setMusicVolume(music_volume_slider.get_current_value() / 100.0)
        #soundWrapper.setAllSoundEffectVolume(sounds, sfx_volume_slider.get_current_value() / 100.0)
        #soundWrapper.setAllVolume(sounds, sfx_volume_slider.get_current_value() / 100.0)
        print('Sound unimplemented')

    menu.process_ui_event = process_ui_event
    return menu