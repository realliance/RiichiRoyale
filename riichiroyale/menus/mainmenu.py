import os
import pygame
import pygame_gui
from .menu import Menu

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