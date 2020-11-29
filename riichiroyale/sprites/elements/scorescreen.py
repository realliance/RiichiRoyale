import os
from pygame.surface import Surface
from pygame import Rect
import numpy
import pygame
from pygame.sprite import LayeredUpdates, Sprite
from pygame.font import Font
from riichiroyale.utils import load_image_resource
from riichiroyale.game.storagemanager import get_object

def render_score_screen(board_render):
    should_show_screen = board_render.board_manager.round_should_end
    group = LayeredUpdates()

    if not should_show_screen:
        return group

    current_path = os.path.dirname(os.path.realpath(__file__))
    font_path = os.path.join(
        current_path, "..", "..", "resources", "fonts", "SourceSans3-Semibold.ttf"
    )
    font = Font(font_path, 40)
    font_small = Font(font_path, 24)

    match = board_render.match
    scores = match.scores
    delta_scores = match.delta_scores
    total_scores = list(map(lambda s: s[0] + s[1], zip(scores, delta_scores)))

    winner_indices = numpy.argwhere(numpy.array(delta_scores) > 0).flatten()
    winner_names = []

    array = numpy.array(total_scores)
    temp = array.argsort()[::-1]
    ranks = numpy.empty_like(temp)
    ranks[temp] = numpy.arange(len(array))

    icons = get_object('boticons')['bot']

    screen_surface = Surface(board_render.surface.get_size(), pygame.SRCALPHA)
    screen_surface.fill((0, 0, 0))

    ROUND_COMPLETE = "Round Complete"

    round_complete_surface = font.render(ROUND_COMPLETE, True, (255, 255, 255))
    font_width, font_height = font.size(ROUND_COMPLETE)

    icon_size = (100, 100)

    player_list = match.ai_list

    x = screen_surface.get_width() // 5
    y = font_height + 5

    for seat in range(4):
        ai_name = player_list[seat]
        name = None
        icon = None
        for entry in icons:
            if entry['ai'] == ai_name:
                name = entry['name']
                icon = entry['icon']
                break
        if name is None:
            raise "BOT WAS NOT DEFINED."

        if seat in winner_indices:
            winner_names += [name]

        icon_surface = Surface(icon_size, pygame.SRCALPHA)
        load_image_resource(icon, icon_surface, size=icon_size)

        screen_surface.blit(
            icon_surface,
            (x, y)
        )

        player_name = font.render(name, True, (255, 255, 255))
        _, name_h = font.size(name)

        screen_surface.blit(
            player_name,
            (x + icon_size[0] + 10, y)
        )

        score_string = "{} Points".format(scores[seat])
        score_render = font_small.render(score_string, True, (255, 255, 255))
        _, score_h = font_small.size(score_string)

        screen_surface.blit(
            score_render,
            (x + icon_size[0] + 10, y + name_h + 5)
        )

        delta_string = "{}{}".format("+" if delta_scores[seat] >= 0 else "", delta_scores[seat])
        delta_render = font_small.render(delta_string, True, (255, 255, 255))
        _, delta_h = font_small.size(delta_string)

        screen_surface.blit(
            delta_render,
            (x + icon_size[0] + 10, y + name_h + 5 + score_h + 5)
        )

        total_string = "Total: {} Points".format(total_scores[seat])
        total_render = font_small.render(total_string, True, (255, 255, 255))

        screen_surface.blit(
            total_render,
            (x + icon_size[0] + 10, y + name_h + 5 + score_h + 5 + delta_h + 5)
        )

        place_string = "{}".format(ranks[seat] + 1)
        place_render = font.render(place_string, True, (255, 255, 255))
        place_w, place_h = font.size(place_string)

        screen_surface.blit(
            place_render,
            (x - place_w - 5, y + ((icon_size[1] - place_h) // 2))
        )

        y += icon_size[1] + 70

    LOADING_NEXT_ROUND = "Loading next Round..."
    loading_surface = font.render(LOADING_NEXT_ROUND, True, (255, 255, 255))
    loading_width, loading_height = font.size(LOADING_NEXT_ROUND)

    screen_surface.blit(
        loading_surface,
        (screen_surface.get_width() - loading_width - 10, screen_surface.get_height() - loading_height - 10)
    )

    screen_surface.blit(
        round_complete_surface,
        ((screen_surface.get_width() // 2) - (font_width // 2), 10),
    )

    result_pos = (screen_surface.get_width() * 0.6, screen_surface.get_height() // 3)

    if board_render.board_manager.did_exhaustive_draw:
        EXHAUSTIVE = "Exhaustive Draw"
        exhaustive_surface = font.render(EXHAUSTIVE, True, (255, 255, 255))

        screen_surface.blit(
            exhaustive_surface,
            result_pos
        )
    else:
        WINNERS = "Winners:"
        winners = ", ".join(winner_names)

        winner_text = font_small.render(WINNERS, True, (255, 255, 255))
        winner_name_text = font_small.render(winners, True, (255, 255, 255))

        screen_surface.blit(
            winner_text,
            result_pos
        )

        screen_surface.blit(
            winner_name_text,
            (result_pos[0], result_pos[1] + winner_text.get_rect().height + 5)
        )




    background_sprite = Sprite()
    background_sprite.rect = screen_surface.get_rect()
    background_sprite.image = screen_surface
    background_sprite.layer = 0
    group.add(background_sprite)



    return group
