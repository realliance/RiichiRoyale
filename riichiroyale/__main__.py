import sys
import math
from pygame import surface, Rect
import pygame
from riichiroyale import Board, Player, build_tile_surface_dict, Tile, BoardRender, TILE_SIZE, SMALL_TILE_SIZE

SCREEN_WIDTH_RATIO, SCREEN_HEIGHT_RATIO = 16, 9

STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT = 1366, 768

def get_play_area_pos(screen):
  w, h = screen.get_size()
  play_area_height = h
  player_area_width = math.floor(h * (float(SCREEN_WIDTH_RATIO)/SCREEN_HEIGHT_RATIO))
  return Rect((w - player_area_width)//2, 0, player_area_width, play_area_height)

def main():
  # Initialize screen
  pygame.init()
  screen = pygame.display.set_mode([STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT])
  pygame.display.set_caption('Riichi Royale')

  # Initialize tile surface dictionary
  tile_dictionary = build_tile_surface_dict('resources/tiles/102x136', TILE_SIZE, file_suffix='png')
  small_tile_dictionary = build_tile_surface_dict('resources/tiles/102x136', SMALL_TILE_SIZE, file_suffix='png')

  # Set Icon
  pygame.display.set_icon(tile_dictionary[Tile.GREEN_DRAGON])

  # Fill background
  background = surface.Surface(screen.get_size())
  background = background.convert_alpha()
  background.fill((7, 99, 36))

  # Play Area Surface
  player_area_rect = get_play_area_pos(screen)
  play_area = surface.Surface((player_area_rect.width, player_area_rect.height), flags=pygame.SRCALPHA)

  current_screen_size = screen.get_size()

  # Example game Board
  main_player = Player("Player")
  bot_one = Player("Bot 1")
  bot_two = Player("Bot 2")
  bot_three = Player("Bot 3")
  board = Board()

  board.register_player(main_player)
  board.register_player(bot_one)
  board.register_player(bot_two)
  board.register_player(bot_three)

  #board.shuffle_dealer()
  main_player.hand = board.draw_tile(num=13)
  main_player.hand.sort()
  bot_one.hand = board.draw_tile(num=13)
  bot_two.hand = board.draw_tile(num=13)
  bot_three.hand = board.draw_tile(num=13)
  board.on_turn()

  board_render = BoardRender(small_tile_dictionary, tile_dictionary, play_area, board, 0)

  # Event loop
  while 1:
    pygame.event.pump()
    for event in pygame.event.get():
      if event.type == pygame.constants.QUIT:
        return

    if screen.get_size() != current_screen_size:
      player_area_rect = get_play_area_pos(screen)
      play_area = surface.Surface((player_area_rect.width, player_area_rect.height), flags=pygame.SRCALPHA)
      board_render.surface = play_area
      board_render.force_redraw()
      background = surface.Surface(screen.get_size())
      background = background.convert_alpha()
      background.fill((7, 99, 36))
      current_screen_size = screen.get_size()

    board_render.update()
    board_render.draw(background)
    screen.blit(background, (0, 0))
    screen.blit(play_area, (player_area_rect.x, player_area_rect.y))
    pygame.display.flip()
    pygame.display.update()

if __name__ == "__main__":
  sys.exit(main())
