import os
import sys
import math
from pygame import surface, Rect
import pygame
from riichiroyale import GameManager, MainMenu, Settings, create_settings_menu, GameView, build_tile_surface_dict, Tile, TILE_SIZE, SMALL_TILE_SIZE, SoundManager

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
  sound_manager.add_audio_source('tileSound', os.path.join(current_path, 'resources/audio/dominoPlace.wav'))
  sound_manager.add_audio_source('shuffleSound', os.path.join(current_path, 'resources/audio/dominoDump.wav'))
  sound_manager.add_audio_source('selected', os.path.join(current_path, 'resources/audio/dominoSelect.wav'))

  sound_manager.add_audio_source('backgroundMusic', os.path.join(current_path, 'resources/audio/peritune-folk-chinese.ogg'), preload=False)

  # Initialize Game Manager and Menus
  game_manager = GameManager(sound_manager)

  main_menu = MainMenu(game_manager, tile_dictionary, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT)
  settings_menu = Settings(game_manager, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT)
  game_view = GameView(sound_manager, screen, tile_dictionary, small_tile_dictionary, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT, SCREEN_WIDTH_RATIO, SCREEN_HEIGHT_RATIO)
  game_manager.add_view(main_menu)
  game_manager.add_view(settings_menu)
  game_manager.add_view(game_view)
  game_manager.set_active_view('main_menu')

  # Clock for pygame-gui
  clock = pygame.time.Clock()

  sound_manager.play_music('backgroundMusic')

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
