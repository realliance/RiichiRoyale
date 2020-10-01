import sys
from pygame import surface
import pygame
import libmahjong
from riichiroyale import Board, Player, build_tile_surface_dict, Tile, BoardRender

SCREEN_WIDTH, SCREEN_HEIGHT = 1280, 720

def main():
    # Initialize screen
    pygame.init()
    screen = pygame.display.set_mode([SCREEN_WIDTH, SCREEN_HEIGHT])
    pygame.display.set_caption('Riichi Royale')

    # Initialize tile surface dictionary
    tile_dictionary = build_tile_surface_dict('build/_deps/riichimahjongtiles-src/Regular', [75,100])

    # Set Icon
    pygame.display.set_icon(tile_dictionary[Tile.GREEN_DRAGON])

    # Fill background
    background = surface.Surface(screen.get_size())
    background = background.convert_alpha()
    background.fill((7, 99, 36))

    # Example game Board
    main_player = Player("Player")
    bot_one = Player("Bot 1")
    bot_two = Player("Bot 2")
    bot_three = Player("Bot 3")
    board = Board(players=[main_player, bot_one, bot_two, bot_three])
    #board.shuffle_dealer()
    main_player.hand = board.draw_tile(num=13)
    bot_one.hand = board.draw_tile(num=13)
    bot_two.hand = board.draw_tile(num=13)
    bot_three.hand = board.draw_tile(num=13)
    board.on_turn()

    board_render = BoardRender(tile_dictionary, screen, board, 0)

    # Blit everything to the screen
    screen.blit(background, (0, 0))
    pygame.display.flip()

    # Event loop
    while 1:
        pygame.event.pump()
        for event in pygame.event.get():
            if event.type == pygame.constants.QUIT:
                return


        screen.blit(background, (0, 0))
        board_render.update()
        board_render.draw()
        pygame.display.flip()
        pygame.display.update()

if __name__ == "__main__":
  sys.exit(main())
