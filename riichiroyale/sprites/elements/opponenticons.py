import pygame
from pygame.sprite import LayeredUpdates, Sprite
from riichiroyale.utils import load_image_resource

def render_bot_icons(board_render):
    ICON_SIZE = (50, 50)
    group = LayeredUpdates()

    SEAT_POS = [
        None,
        (board_render.surface.get_width() - ICON_SIZE[0] - 10, 80),
        (460 - ICON_SIZE[0] - 10, 10),
        (10, 80)
    ]

    game_view = board_render.match.game_manager.get_active_view()

    icon_cache = game_view.bot_icon_cache
    ais = game_view.ai_list
    ai_icons = []
    dud_ai_icon = pygame.Surface(ICON_SIZE, pygame.SRCALPHA)
    load_image_resource("dudB.png", dud_ai_icon, size=ICON_SIZE)
    for i in range(4):
        for icon in icon_cache:
            if icon['ai'] == ais[i]:
                surface = pygame.Surface(ICON_SIZE, pygame.SRCALPHA)
                load_image_resource(icon['icon'], surface, size=ICON_SIZE)
                ai_icons += [surface]
                break

    is_story_mode = hasattr(game_view, "match_dict")
    for seat in range(4):
        if seat == 0:
            continue
        sprite = Sprite()
        sprite.rect = (SEAT_POS[seat], ICON_SIZE)
        sprite.layer = 0
        if is_story_mode and seat != 2:
            sprite.image = dud_ai_icon
        else:
            sprite.image = ai_icons[seat]
        group.add(sprite)
    return group