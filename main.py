import sys
from pygame import surface
import pygame
import pygame_gui
import libmahjong
from riichiroyale import MahjongHand, load_image, load_tile

SCREEN_WIDTH, SCREEN_HEIGHT = 1500, 400


def _build_icon():
    front = load_image('resources/tiles/51x68/Chun.png', convert=False)
    back = load_image('resources/tiles/51x68/Front.png', convert=False)
    return load_tile(front, back)


def main():
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

    # Setup pygame-gui
    manager = pygame_gui.UIManager((SCREEN_WIDTH, SCREEN_HEIGHT), 'theme.json')
    newgame_button = pygame_gui.elements.UIButton(relative_rect=pygame.Rect((350, 275), (100, 50)),
                                                  text='New Game',
                                                  manager=manager)

    # Display some text
    font = pygame.font.Font('build/_deps/sourcesanspro-src/TTF/SourceSans3-Regular.ttf', 36)
    text = font.render("Press Space to Refresh Hand", 1, (255, 255, 255))
    textpos = text.get_rect()
    textpos.bottomright = background.get_rect().bottomright
    textpos.right -= 10
    background.blit(text, textpos)

    libmahjong.Walls.SetPath('resources/tiles/102x136')
    hand = MahjongHand()

    # Blit everything to the screen
    screen.blit(background, (0, 0))
    pygame.display.flip()

    space_pressed = False

    # Clock for pygame-gui
    clock = pygame.time.Clock()

    # Event loop
    while 1:
        time_delta = clock.tick(60) / 1000.0
        pygame.event.pump()
        keys = pygame.key.get_pressed()
        for event in pygame.event.get():
            if event.type == pygame.constants.QUIT:
                return
            if event.type == pygame.USEREVENT:
                if event.user_type == pygame_gui.UI_BUTTON_PRESSED:
                    if event.ui_element == newgame_button:
                        print('Clicked new game button.')
            manager.process_events(event)
        if keys[pygame.constants.K_SPACE] and not space_pressed:
            space_pressed = True
            hand.newHand()
        if not keys[pygame.constants.K_SPACE]:
            space_pressed = False
        manager.update(time_delta)
        screen.blit(background, (0, 0))
        hand.update()
        hand.draw(screen)
        manager.draw_ui(screen)
        pygame.display.flip()
        pygame.display.update()


if __name__ == "__main__":
    sys.exit(main())
