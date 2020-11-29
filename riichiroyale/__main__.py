import os
import sys
import math
import logging
import pygame
from pygame import surface, Rect
from pygame.font import Font
from libmahjong import register_ai, avaliable_ais, unregister_ai, PieceType
from riichiroyale import (
    GameManager,
    MainMenu,
    Settings,
    FreePlaySelect,
    GameView,
    build_tile_surface_dict,
    TILE_SIZE,
    SMALL_TILE_SIZE,
    SoundManager,
    BoardManager,
    TutorialView,
    PlayerManager,
    bootstrap_base_storage,
    get_object,
    StoryModeSelect,
    StoryModeGame,
    StubbornBot
)

SCREEN_WIDTH_RATIO, SCREEN_HEIGHT_RATIO = 16, 9
STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT = 1366, 768

def get_play_area_pos(screen):
    w, h = screen.get_size()
    play_area_height = h
    player_area_width = math.floor(
        h * (float(SCREEN_WIDTH_RATIO) / SCREEN_HEIGHT_RATIO)
    )
    return Rect((w - player_area_width) // 2, 0, player_area_width, play_area_height)

def draw_loading_screen(screen, background, font, text):
    # Draw black screen so window properly renders
    screen.blit(background, (0, 0))

    font_surface = font.render(text, True, (255, 255, 255))
    _, screen_height = screen.get_size()
    _, font_height = font.size(text)
    screen.blit(
        font_surface,
        (20, screen_height - font_height - 20),
    )

    pygame.display.flip()
    pygame.display.update()

def main():
    # Initialize screen
    pygame.init()
    screen = pygame.display.set_mode([STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT])
    pygame.display.set_caption("Riichi Royale")

    current_path = os.path.dirname(os.path.realpath(__file__))

    bootstrap_base_storage()

    # Init Logging
    logging.basicConfig(
        format="%(asctime)s %(levelname)s: %(message)s", datefmt="%m/%d/%Y %I:%M:%S %p"
    )

    # Loading Font
    font_path = os.path.join(
        current_path, "resources", "fonts", "SourceSans3-Semibold.ttf"
    )
    font = Font(font_path, 40)

    # Fill background
    clear_background = surface.Surface(screen.get_size())
    clear_background = clear_background.convert_alpha()
    clear_background.fill((0, 0, 0))
    
    draw_loading_screen(screen, clear_background, font, "Loading Tiles...")

    # Initialize tile surface dictionary
    tile_dictionary = build_tile_surface_dict(
        os.path.join(current_path, "resources", "tiles", "102x136"),
        TILE_SIZE,
        file_suffix="png",
    )
    small_tile_dictionary = build_tile_surface_dict(
        os.path.join(current_path, "resources", "tiles", "102x136"),
        SMALL_TILE_SIZE,
        file_suffix="png",
    )

    draw_loading_screen(screen, clear_background, font, "Registering Player Interface...")

    # Register Python AIs
    player_manager = PlayerManager()
    register_ai(player_manager, "Player")

    stubborn_bot = StubbornBot()
    register_ai(stubborn_bot, "StubbornBot")

    print(avaliable_ais())

    # Set Icon
    pygame.display.set_icon(tile_dictionary[PieceType.GREEN_DRAGON])

    draw_loading_screen(screen, clear_background, font, "Loading Lofi Jams...")

    # Initialize Sound Manager
    sound_manager = SoundManager()

    sound_manager.register_sfx_set("clack")

    # Init Clacks
    for x in range(1, 18):
        name = "clack{0}".format(x)
        sound_manager.add_audio_source(
            name,
            os.path.join(
                current_path,
                "resources",
                "audio",
                "tileclack",
                "clack-{:02}.ogg".format(x),
            ),
        )
        sound_manager.add_to_sfx_set("clack", name)

    sound_manager.add_audio_source(
        "lobby",
        os.path.join(current_path, "resources", "audio", "music", "lobby.ogg"),
        preload=False,
    )
    sound_manager.add_audio_source(
        "game1",
        os.path.join(current_path, "resources", "audio", "music", "game1.ogg"),
        preload=False,
    )
    sound_manager.add_audio_source(
        "game2",
        os.path.join(current_path, "resources", "audio", "music", "game2.ogg"),
        preload=False,
    )
    sound_manager.add_audio_source(
        "game3",
        os.path.join(current_path, "resources", "audio", "music", "game3.ogg"),
        preload=False,
    )

    sound_manager.music_playlist = ["game1", "game2", "game3"]

    draw_loading_screen(screen, clear_background, font, "Prepping Game Views...")

    # Init Board Manager
    board_manager = BoardManager()

    # Initialize Game Manager and Menus
    game_manager = GameManager(board_manager, sound_manager)

    main_menu = MainMenu(
        game_manager, tile_dictionary, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT
    )
    freeplay_menu = FreePlaySelect(
        game_manager, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT
    )
    storymode_menu = StoryModeSelect(
        game_manager, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT
    )
    settings_menu = Settings(
        game_manager, STARTING_SCREEN_WIDTH, STARTING_SCREEN_HEIGHT
    )
    game_view = GameView(
        game_manager,
        screen,
        tile_dictionary,
        small_tile_dictionary,
        STARTING_SCREEN_WIDTH,
        STARTING_SCREEN_HEIGHT,
        SCREEN_WIDTH_RATIO,
        SCREEN_HEIGHT_RATIO,
        player_manager=player_manager,
    )
    storymode_game_view = StoryModeGame(
        game_manager,
        screen,
        tile_dictionary,
        small_tile_dictionary,
        STARTING_SCREEN_WIDTH,
        STARTING_SCREEN_HEIGHT,
        SCREEN_WIDTH_RATIO,
        SCREEN_HEIGHT_RATIO,
        player_manager=player_manager,
    )
    tutorial_view = TutorialView(
        game_manager,
        screen,
        tile_dictionary,
        small_tile_dictionary,
        STARTING_SCREEN_WIDTH,
        STARTING_SCREEN_HEIGHT,
        SCREEN_WIDTH_RATIO,
        SCREEN_HEIGHT_RATIO,
        player_manager=player_manager,
    )

    game_manager.add_view(main_menu)
    game_manager.add_view(settings_menu)
    game_manager.add_view(game_view)
    game_manager.add_view(tutorial_view)
    game_manager.add_view(freeplay_menu)
    game_manager.add_view(storymode_menu)
    game_manager.add_view(storymode_game_view)
    game_manager.set_active_view("main_menu")

    draw_loading_screen(screen, clear_background, font, "Done!")

    sound_manager.play_music("lobby")

    # Clock for pygame-gui
    clock = pygame.time.Clock()

    running = True

    # Event loop
    while running:
        time_delta = clock.tick(60) / 1000.0
        pygame.event.pump()
        for event in pygame.event.get(pump=False):
            if event.type == pygame.QUIT:
                running = False
            game_manager.on_pygame_event(event)

        game_manager.update(time_delta)
        screen.blit(clear_background, (0, 0))
        game_manager.draw(screen)
        pygame.display.flip()
        pygame.display.update()

    for view in game_manager.views:
        if hasattr(game_manager.views[view], 'match'):
            if game_manager.views[view].match is not None:
                game_manager.views[view].match.match_alive = False
                with game_manager.views[view].match.process_lock:
                    game_manager.views[view].match.process_lock.notify()

    unregister_ai("Player")
    unregister_ai("StubbornBot")

    pygame.quit()

if __name__ == "__main__":
    sys.exit(main())
