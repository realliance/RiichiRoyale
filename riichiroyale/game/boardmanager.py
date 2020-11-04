from libmahjong import EventType
from .gameevent import Event, CombinedEventType
from .meld import Meld
from .call import Call, CallDirection


class BoardManager:
    def __init__(self):
        self.last_event = None
        self.last_decision_event = None
        self.waiting_on_decision = False
        self.round_should_end = False
        self.game_should_end = False


def process_event_queue(game_manager, match, current_queue):
    if len(current_queue) == 0:
        return
    event = current_queue.pop(0)
    if event.type == EventType.Discard and len(current_queue) > 0:
        next_event = current_queue.pop(0)
        if (
            next_event.type
            in (EventType.Pon, EventType.Ron, EventType.Kan, EventType.Chi)
            and event.player == next_event.player
        ):
            new_event = Event(
                CombinedEventType(100 + int(next_event.type)),
                event.player,
                event.piece,
                next_event.decision,
                extra_player=next_event.player,
                extra_piece=next_event.piece,
                raw_event_a=event,
                raw_event_b=next_event,
            )
            process_event_queue(game_manager, match, [new_event] + current_queue)
            return
        current_queue = [next_event] + current_queue
    on_game_event(game_manager, event, match)
    process_event_queue(game_manager, match, current_queue)


def on_chi_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    match,
    event_player,
    _pov_player,
    _extra_player,
):
    if not is_decision:
        last_event = game_manager.board_manager.last_decision_event
        del match.current_board.players[last_event.player].discard_pile[-1]
        event_player.hand += [last_event.extra_piece]

        game_manager.board_manager.last_decision_event = None
        game_manager.board_manager.waiting_on_decision = False

        match.current_board.current_turn = event.player

        if is_ai:
            del event_player.hand[-3:]
        else:
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece + 1)
            event_player.hand.remove(event.piece + 2)

        event_player.melded_hand += [
            Meld([event.piece, event.piece + 1, event.piece + 2], CallDirection.Left)
        ]
        event_player.calls_avaliable = []


def on_pon_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    match,
    event_player,
    _pov_player,
    _extra_player,
):
    if not is_decision:
        last_event = game_manager.board_manager.last_decision_event
        del match.current_board.players[last_event.player].discard_pile[-1]

        if is_ai:
            del event_player.hand[-2:]
        else:
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece)

        match.current_board.current_turn = event.player
        event_player.melded_hand += [
            Meld(
                [event.piece] * 3,
                CallDirection.get_call_direction(
                    event_player.player, last_event.player
                ),
            )
        ]
        event_player.calls_avaliable = []

        game_manager.board_manager.last_decision_event = None
        game_manager.board_manager.waiting_on_decision = False


def on_ron_event(
    game_manager,
    _is_ai,
    is_decision,
    event,
    _match,
    event_player,
    _pov_player,
    _extra_player,
):
    if not is_decision:
        event_player.hand += [event.piece]
        event_player.calls_avaliable = []
        game_manager.board_manager.waiting_on_decision = False
        game_manager.board_manager.round_should_end = True


def on_converted_kan_event(
    game_manager,
    _is_ai,
    is_decision,
    event,
    _match,
    event_player,
    _pov_player,
    _extra_player,
):
    if is_decision:
        game_manager.board_manager.waiting_on_decision = True
        game_manager.board_manager.last_decision_event = event
        event_player.calls_avaliable += [Call.Kan]
    else:
        # Converted Kan
        for melds in event_player.melded_hand:
            if melds.tiles == [event.piece] * 3:
                melds.tiles += [event.piece]
                melds.converted_kan = True
                break

        event_player.hand.remove(event.piece)
        event_player.calls_avaliable = []
        game_manager.board_manager.last_decision_event = None
        game_manager.board_manager.waiting_on_decision = False


def on_kan_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    match,
    event_player,
    _pov_player,
    _extra_player,
):
    if not is_decision:
        # Calling Kan
        last_event = game_manager.board_manager.last_decision_event
        del match.current_board.players[last_event.player].discard_pile[-1]

        match.current_board.current_turn = event.player

        event_player.melded_hand += [
            Meld(
                [event.piece] * 4,
                CallDirection.get_call_direction(event.player, last_event.player),
            )
        ]

        if is_ai:
            del event_player.hand[-3:]
        else:
            event_player.calls_avaliable = []
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece)
            game_manager.board_manager.last_decision_event = None
            game_manager.board_manager.waiting_on_decision = False


def on_tsumo_event(
    game_manager,
    _is_ai,
    is_decision,
    _event,
    _match,
    event_player,
    _pov_player,
    _extra_player,
):
    if is_decision:
        game_manager.board_manager.waiting_on_decision = True
        event_player.calls_avaliable += [Call.Tsumo]
    else:
        event_player.calls_avaliable = []
        game_manager.board_manager.waiting_on_decision = False
        game_manager.board_manager.round_should_end = True


def on_riichi_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    match,
    event_player,
    _pov_player,
    _extra_player,
):
    if is_decision:
        game_manager.board_manager.waiting_on_decision = True
        game_manager.board_manager.last_decision_event = event
        event_player.calls_avaliable += [Call.Riichi]
    else:
        if is_ai:
            del event_player.hand[-1]
        else:
            event_player.hand.remove(event.piece)
        game_manager.board_manager.waiting_on_decision = False
        game_manager.board_manager.last_decision_event = None
        event_player.calls_avaliable += []
        event_player.discard_pile += [event.piece]
        event_player.riichi_declared = True
        match.current_board.next_turn()


def on_discard_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    match,
    event_player,
    _pov_player,
    _extra_player,
):
    if is_decision:
        if (
            game_manager.board_manager.last_event is None
            or game_manager.board_manager.last_event.type
            not in (EventType.Chi, EventType.Pon, EventType.Kan)
        ):
            event_player.hand += [event.piece]
            if len(match.current_board.wall) > 0:
                del match.current_board.wall[-1]
        event_player.my_turn = True
        game_manager.board_manager.waiting_on_decision = True
    else:
        if not is_ai:
            event_player.hand.remove(event.piece)
            event_player.hand.sort()
        elif len(match.current_board.wall) > 0:
            del match.current_board.wall[-1]
        event_player.discard_pile += [event.piece]


def on_concealed_kan_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    _match,
    event_player,
    _pov_player,
    _extra_player,
):
    if is_decision:
        game_manager.board_manager.waiting_on_decision = True
        event_player.calls_avaliable += [Call.Concealed_Kan]
    else:
        if is_ai:
            del event_player.hand[-4:]
        else:
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece)

        game_manager.board_manager.waiting_on_decision = False
        event_player.calls_avaliable = []
        event_player.melded_hand += [Meld([event.piece] * 4, CallDirection.Concealed)]


def on_decline_event(
    _game_manager,
    _is_ai,
    _is_decision,
    _event,
    _match,
    _event_player,
    _pov_player,
    _extra_player,
):
    return


def on_dora_event(
    _game_manager,
    _is_ai,
    _is_decision,
    event,
    match,
    _event_player,
    _pov_player,
    _extra_player,
):
    match.current_board.deadwall[match.current_board.dora_revealed * 2] = event.piece
    match.current_board.dora_revealed += 1


def on_point_diff_event(
    game_manager,
    _is_ai,
    _is_decision,
    event,
    match,
    _event_player,
    _pov_player,
    _extra_player,
):
    match.scores[event.player] += event.piece * 1000
    game_manager.board_manager.round_should_end = True


def on_end_event(
    game_manager,
    _is_ai,
    _is_decision,
    _event,
    _match,
    _event_player,
    _pov_player,
    _extra_player,
):
    game_manager.board_manager.game_should_end = True


def on_discard_chi_event(
    game_manager,
    _is_ai,
    is_decision,
    event,
    match,
    event_player,
    pov_player,
    extra_player,
):
    if is_decision:
        event_player.discard_pile += [event.piece]

        game_manager.board_manager.last_decision_event = event
        game_manager.board_manager.waiting_on_decision = True
        pov_player.calls_avaliable += [Call.Chi]
    else:
        del extra_player.hand[-2:]
        chi_tile = event.extra_piece
        extra_player.melded_hand += [
            Meld([chi_tile, chi_tile + 1, chi_tile + 2], CallDirection.Left)
        ]
        match.current_board.current_turn = event.extra_player


def on_discard_kan_event(
    game_manager,
    _is_ai,
    is_decision,
    event,
    match,
    event_player,
    pov_player,
    extra_player,
):
    if is_decision:
        event_player.discard_pile += [event.piece]

        game_manager.board_manager.last_decision_event = event
        game_manager.board_manager.waiting_on_decision = True
        pov_player.calls_avaliable += [Call.Kan]
    else:
        del extra_player.hand[-3:]
        kan_tile = event.extra_piece

        extra_player.melded_hand += [
            Meld(
                [kan_tile] * 4,
                CallDirection.get_call_direction(event.player, event.extra_player),
            )
        ]
        match.current_board.current_turn = event.extra_player


def on_discard_pon_event(
    game_manager,
    _is_ai,
    is_decision,
    event,
    match,
    event_player,
    pov_player,
    extra_player,
):
    if is_decision:
        event_player.discard_pile += [event.piece]

        game_manager.board_manager.last_decision_event = event
        game_manager.board_manager.waiting_on_decision = True
        pov_player.calls_avaliable += [Call.Pon]
    else:
        del extra_player.hand[-2:]
        pon_tile = event.extra_piece
        extra_player.melded_hand += [
            Meld(
                [pon_tile] * 3,
                CallDirection.get_call_direction(event.extra_player, event.player),
            )
        ]
        match.current_board.current_turn = event.extra_player


def on_discard_ron_event(
    game_manager,
    _is_ai,
    is_decision,
    event,
    _match,
    event_player,
    pov_player,
    extra_player,
):
    if is_decision:
        event_player.discard_pile += [event.piece]

        game_manager.board_manager.last_decision_event = event
        game_manager.board_manager.waiting_on_decision = True
        pov_player.calls_avaliable += [Call.Ron]
    else:
        extra_player.hand += [event.piece]
        game_manager.board_manager.waiting_on_decision = False
        game_manager.board_manager.round_should_end = True


def on_game_event(game_manager, event, match):
    is_ai = event.player != match.player_id
    is_decision = event.decision
    event_type = event.type

    EVENTS = {
        EventType.Chi: on_chi_event,
        EventType.Pon: on_pon_event,
        EventType.Ron: on_ron_event,
        EventType.ConvertedKan: on_converted_kan_event,
        EventType.Kan: on_kan_event,
        EventType.Tsumo: on_tsumo_event,
        EventType.Riichi: on_riichi_event,
        EventType.Discard: on_discard_event,
        EventType.Decline: on_decline_event,
        EventType.Dora: on_dora_event,
        EventType.PointDiff: on_point_diff_event,
        EventType.End: on_end_event,
        CombinedEventType.DiscardChi: on_discard_chi_event,
        CombinedEventType.DiscardKan: on_discard_kan_event,
        CombinedEventType.DiscardPon: on_discard_pon_event,
        CombinedEventType.DiscardRon: on_discard_ron_event,
    }

    actual_player = match.current_board.players[match.player_id]

    if event.player is not None or event.player in range(4):
        player = match.current_board.players[event.player]
    else:
        player = None

    if hasattr(event, "extra_player"):
        if event.extra_player is not None or event.extra_player in range(4):
            extra_player = match.current_board.players[event.extra_player]
        else:
            extra_player = None
    else:
        extra_player = None

    # Align Turn
    if event_type in (
        EventType.Discard,
        CombinedEventType.DiscardChi,
        CombinedEventType.DiscardKan,
        CombinedEventType.DiscardPon,
        CombinedEventType.DiscardRon,
    ):
        match.current_board.current_turn = event.player

    EVENTS[event_type](
        game_manager,
        is_ai,
        is_decision,
        event,
        match,
        player,
        actual_player,
        extra_player,
    )
    game_manager.board_manager.last_event = event
