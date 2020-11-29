import copy
from pygame.sprite import Group
from .elements import (
    render_center_info,
    render_hand,
    render_meld_hand,
    render_discard_pile,
    render_vertical_discard_pile,
    render_dora_pile,
    render_hidden_hand,
    render_score_screen,
    render_bot_icons
)

class StatefulBoardElement:
    def __init__(self, default_value, get_value, render):
        self.value = copy.copy(default_value)
        self.get_value = get_value
        self.render = render
        self.rendered = Group()

    def notify(self, surface, background):
        if self.value != self.get_value():
            self.rendered.clear(surface, background)
            self.value = copy.copy(self.get_value())
            self.rendered = self.render()

    def force_render(self):
        self.rendered = self.render()

    def draw(self, surface, background):
        self.rendered.clear(surface, background)
        self.rendered.draw(surface)

    def update(self, callback_handler=None):
        self.rendered.update(callback_handler)


def calculate_against_player_pov(player_pov, offset):
    return (player_pov + offset) % 4


class BoardRender:
    def __init__(self, small_dictionary, dictionary, surface, match, player_pov, board_manager):
        self.match = match
        self.surface = surface
        self.small_dictionary = small_dictionary
        self.dictionary = dictionary
        self.player_pov = player_pov
        self.player_bound_elements = []
        self.elements = []
        self.board_manager = board_manager

        # Player Hand
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[player_pov].hand,
                lambda: render_hand(self, player_pov),
            )
        )

        player_to_right = calculate_against_player_pov(player_pov, 1)
        player_across = calculate_against_player_pov(player_pov, 2)
        player_to_left = calculate_against_player_pov(player_pov, 3)

        # Opponent Hands
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[1].hand,
                lambda: render_hidden_hand(self, player_to_right, 1),
            )
        )
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[2].hand,
                lambda: render_hidden_hand(self, player_across, 2),
            )
        )
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[3].hand,
                lambda: render_hidden_hand(self, player_to_left, 3),
            )
        )

        # Player Melds
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: meld_state(self.match.players[player_pov].melded_hand),
                lambda: render_meld_hand(
                    self, self.match.players[player_pov].melded_hand, seat=0
                ),
            )
        )

        # Opponent Melds
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: meld_state(self.match.players[player_to_right].melded_hand),
                lambda: render_meld_hand(
                    self, self.match.players[player_to_right].melded_hand, seat=1
                ),
            )
        )
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: meld_state(self.match.players[player_across].melded_hand),
                lambda: render_meld_hand(
                    self, self.match.players[player_across].melded_hand, seat=2
                ),
            )
        )
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: meld_state(self.match.players[player_to_left].melded_hand),
                lambda: render_meld_hand(
                    self, self.match.players[player_to_left].melded_hand, seat=3
                ),
            )
        )

        # Discard Piles
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[player_pov].discard_pile,
                lambda: render_discard_pile(self, player_pov, 0),
            )
        )
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[player_to_right].discard_pile,
                lambda: render_vertical_discard_pile(self, player_to_right, 1),
            )
        )
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[player_across].discard_pile,
                lambda: render_discard_pile(self, player_across, 2),
            )
        )
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.players[player_to_left].discard_pile,
                lambda: render_vertical_discard_pile(self, player_to_left, 3),
            )
        )

        # Center Info
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: center_info_state(self.match),
                lambda: render_center_info(self),
            )
        )

        # AI Portraits
        self.elements.append(
            StatefulBoardElement(
                [],
                lambda: self.match.ai_list,
                lambda: render_bot_icons(self),
            )
        )

        # Doras
        self.elements.append(
            StatefulBoardElement(
                0, lambda: self.match.current_board.dora_revealed, lambda: render_dora_pile(self)
            )
        )

        # End Screen
        self.elements.append(
            StatefulBoardElement(
                False, lambda: board_manager.round_should_end, lambda: render_score_screen(self)
            )
        )

    def update(self, callback_handler=None):
        for element in self.elements:
            element.update(callback_handler=callback_handler)

    def force_redraw(self):
        for element in self.elements:
            element.force_render()

    def draw(self, background):
        with self.match.players[self.player_pov].lock:
            for element in self.elements:
                element.notify(self.surface, background)
                element.draw(self.surface, background)

def meld_state(meld_list):
    return list(map(lambda meld: list(map(lambda tile: tile.get_raw_value(), meld.tiles)), meld_list))

def center_info_state(match):
    return [len(match.current_board.wall), match.current_board.current_dealer, match.current_board.current_turn]
