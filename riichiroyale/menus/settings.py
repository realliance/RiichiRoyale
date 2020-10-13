import os
import pygame
import pygame_gui
from .menu import Menu

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