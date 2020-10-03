import sys
import enum
from math import cos, sin
from pygame import surface
import pygame
import pygame_gui
import libmahjong
from riichiroyale import MahjongHand, load_image, load_tile

SCREEN_WIDTH, SCREEN_HEIGHT = 1500, 800


class Menu:
    uiElements = list()
    manager = pygame_gui.UIManager

    def processuievent(self, event):
        print('Got uncaught UI event: ', event)


currentMenu = Menu()
menus = {

}


def _build_icon():
    front = load_image('resources/tiles/51x68/Chun.png', convert=False)
    back = load_image('resources/tiles/51x68/Front.png', convert=False)
    return load_tile(front, back)


def createmainmenu():
    menu = Menu()
    menu.manager = pygame_gui.UIManager((SCREEN_WIDTH, SCREEN_HEIGHT), 'theme.json')
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
    title_rect.center = (SCREEN_WIDTH/2, SCREEN_HEIGHT/2)
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
    def processuievent(event):
        global currentMenu
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == newgame_button:
                print('Pressed new game')
            if event.ui_element == settings_button:
                print('Switching to settings menu')
                currentMenu = menus['settings']

    menu.processuievent = processuievent
    return menu


def createsettingsmenu():
    menu = Menu()
    menu.manager = pygame_gui.UIManager((SCREEN_WIDTH, SCREEN_HEIGHT), 'theme.json')

    settingsmenu_rect = pygame.Rect(SCREEN_WIDTH / 2 - 500, SCREEN_HEIGHT / 2 - 350, 1000, 700)
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

    settings_label_rect = pygame.Rect(0, 0, 200, 50)
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

    menu.uiElements.append(settingsmenu_panel)
    menu.uiElements.append(back_button)
    menu.uiElements.append(settings_label)

    # processuievent() is called when a UI event is caught while this menu is active
    def processuievent(event):
        global currentMenu
        if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
            if event.ui_element == back_button:
                print('Pressed back')
                currentMenu = menus['main']

    menu.processuievent = processuievent
    return menu


def main():
    global currentMenu
    # Initialise screen
    pygame.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption('Riichi Royale')

    # Set Icon
    pygame.display.set_icon(_build_icon())

    # Fill background
    background = surface.Surface(screen.get_size())
    background = background.convert_alpha()
    background.fill((7, 99, 36))

    # Setup menus
    mainmenu = createmainmenu()
    settingsmenu = createsettingsmenu()
    menus['main'] = mainmenu
    menus['settings'] = settingsmenu

    # Display some text
    font = pygame.font.Font('build/_deps/sourcesanspro-src/TTF/SourceSans3-Regular.ttf', 36)
    text = font.render("Press Space to Refresh Hand", 1, (255, 255, 255))
    textpos = text.get_rect()
    textpos.bottomright = background.get_rect().bottomright
    textpos.right -= 10

    titlefont = pygame.font.Font('build/_deps/sourcesanspro-src/TTF/SourceSans3-BoldIt.ttf', 100)
    titletext = titlefont.render("Riichi Royale", 1, (255, 40, 40))
    titletextpos = titletext.get_rect(center=(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2))

    libmahjong.Walls.SetPath('resources/tiles/102x136')
    hand = MahjongHand()

    # Blit everything to the screen
    screen.blit(background, (0, 0))
    pygame.display.flip()

    space_pressed = False

    # Clock for pygame-gui
    clock = pygame.time.Clock()

    # Menu to draw
    currentMenu = mainmenu

    # Event loop
    while 1:
        time_delta = clock.tick(60) / 1000.0
        pygame.event.pump()
        keys = pygame.key.get_pressed()
        for event in pygame.event.get():
            if event.type == pygame.constants.QUIT:
                return
            # Pass UI events to the current menu for processing
            if event.type == pygame.USEREVENT:
                currentMenu.processuievent(event)
            # Pass window events to pygame-gui for processing
            currentMenu.manager.process_events(event)
        if keys[pygame.constants.K_SPACE] and not space_pressed:
            space_pressed = True
            hand.newHand()
        if not keys[pygame.constants.K_SPACE]:
            space_pressed = False
        # Update the state of the current menu
        currentMenu.manager.update(time_delta)
        screen.blit(background, (0, 0))
        if currentMenu == menus['main']:
            hand.update()
            for index, sprite in enumerate(hand.sprites()):
                sprite.rect = ((SCREEN_WIDTH / 2 - 100) * cos(index + pygame.time.get_ticks() / 5000.0) + 700,
                               (SCREEN_HEIGHT / 2 - 100) * sin(index + pygame.time.get_ticks() / 5000.0) + 350)
            hand.draw(screen)
        currentMenu.manager.draw_ui(screen)
        pygame.display.flip()
        pygame.display.update()


if __name__ == "__main__":
    sys.exit(main())
