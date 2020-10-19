import os
import sys
import math
from libmahjong import MahjongGameManager
from pygame import surface, Rect
import pygame
from riichiroyale import GameManager, MainMenu, Settings, create_settings_menu, GameView, build_tile_surface_dict, Tile, TILE_SIZE, SMALL_TILE_SIZE, SoundManager, BoardManager

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

  current_path = os.path.dirname(os.path.realpath(__file__))

  # Initialize tile surface dictionary
  tile_dictionary = build_tile_surface_dict(os.path.join(current_path, 'resources/tiles/102x136'), TILE_SIZE, file_suffix='png')
  small_tile_dictionary = build_tile_surface_dict(os.path.join(current_path, 'resources/tiles/102x136'), SMALL_TILE_SIZE, file_suffix='png')

  # Set Icon
  pygame.display.set_icon(tile_dictionary[Tile.GREEN_DRAGON])

  # Fill background
  clear_background = surface.Surface(screen.get_size())
  clear_background = clear_background.convert_alpha()
  clear_background.fill((0, 0, 0))

  # Initialize Sound Manager
  sound_manager = SoundManager()

  sound_manager.register_sfx_set('clack')

  # Init Clacks
  for x in range(1, 18):
    name = 'clack{0}'.format(x)
    sound_manager.add_audio_source(name, os.path.join(current_path, 'resources/audio/tileclack/clack-{:02}.ogg'.format(x)))
    sound_manager.add_to_sfx_set('clack', name)

  
  sound_manager.add_audio_source('lobby', os.path.join(current_path, 'resources/audio/music/lobby.ogg'), preload=False)
  sound_manager.add_audio_source('game1', os.path.join(current_path, 'resources/audio/music/game1.ogg'), preload=False)
  sound_manager.add_audio_source('game2', os.path.join(current_path, 'resources/audio/music/game2.ogg'), preload=False)
  sound_manager.add_audio_source('game3', os.path.join(current_path, 'resources/audio/music/game3.ogg'), preload=False)

  sound_manager.music_playlist = ['game1', 'game2', 'game3']

  # Init Board Manager
  board_manager = BoardManager()

  # Initialize Game Manager and Menus
  game_manager = GameManager(board_manager, sound_manager)

  main_menu = MainMenu(game_manager, tile_dictionary, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT)
  settings_menu = Settings(game_manager, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT)
  game_view = GameView(game_manager, sound_manager, screen, tile_dictionary, small_tile_dictionary, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT, SCREEN_WIDTH_RATIO, SCREEN_HEIGHT_RATIO)
  tutorial_view = GameView(game_manager, sound_manager, screen, tile_dictionary, small_tile_dictionary, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT, SCREEN_WIDTH_RATIO, SCREEN_HEIGHT_RATIO, name="tutorial")
  
  game_manager.add_view(main_menu)
  game_manager.add_view(settings_menu)
  game_manager.add_view(game_view)
  game_manager.add_view(tutorial_view)
  game_manager.set_active_view('main_menu')

  # Clock for pygame-gui
  clock = pygame.time.Clock()

  sound_manager.play_music('lobby')

  # Event loop
  while 1:
    time_delta = clock.tick(60) / 1000.0
    pygame.event.pump()
    for event in pygame.event.get():
        if event.type == pygame.constants.QUIT:
            return
        game_manager.on_pygame_event(event)

    game_manager.update(time_delta)
    screen.blit(clear_background, (0, 0))
    game_manager.draw(screen)
    pygame.display.flip()
    pygame.display.update()

if __name__ == "__main__":
  sys.exit(main())
