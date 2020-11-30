from time import sleep
from copy import copy
from libmahjong import EventType, Piece, PieceType
from .gameevent import Event, CombinedEventType
from .meld import Meld
from .call import Call, CallDirection


class BoardManager:
    def __init__(self):
        self.last_event = None
        self.last_decision_event = None
        self.waiting_on_decision = False
        self._point_diff_count = 0
        self.round_should_end = False
        self.did_exhaustive_draw = True
        self.game_should_end = False

def convert_event(event):
    if isinstance(event.type, CombinedEventType):
        return event

    event_piece = event.piece.get_raw_value() if isinstance(event.piece, Piece) else event.piece
    event_piece_value = int(event_piece)
    print(event_piece_value)
    if event_piece_value >= 0:
        return Event(event.type, event.player, Piece(event_piece_value), event.decision)

    return Event(event.type, event.player, event_piece_value, event.decision)

def process_event_queue(game_manager, match):
    event = match.player_manager.Pop()
    if event.type == EventType.End:
        match.player_manager.encountered_end_game = True
    if event is None:
        return
    
    event = convert_event(event)
    if event.type == EventType.Discard and match.player_manager.GetQueueLength() > 0:
        next_event = match.player_manager.Pop()
        next_event = convert_event(next_event)
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
                raw_event_a=copy(event),
                raw_event_b=copy(next_event),
            )
            match.player_manager.AddToEventQueue(new_event)
            process_event_queue(game_manager, match)
            return
        match.player_manager.AddToEventQueue(next_event)
    on_game_event(game_manager, event, match)


def on_chi_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    match,
    event_player,
    pov_player,
    _extra_player,
):
    if not is_decision:
        if is_ai:
            last_event = game_manager.board_manager.last_event
        else:
            last_event = game_manager.board_manager.last_decision_event
        del match.players[last_event.player].discard_pile[-1]
        event_player.hand += [last_event.extra_piece]

        game_manager.board_manager.last_decision_event = None
        game_manager.board_manager.waiting_on_decision = False

        match.current_board.current_turn = event.player

        if is_ai:
            del event_player.hand[-4:]
            print(len(event_player.hand))
        else:
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece + 1)
            event_player.hand.remove(event.piece + 2)

        piece_list = list(filter(lambda piece: piece != last_event.piece, [event.piece, event.piece + 1, event.piece + 2]))
        
        event_player.melded_hand += [
            Meld([last_event.piece] + piece_list, CallDirection.Left)
        ]
        event_player.calls_avaliable = []

        game_manager.board_manager.last_decision_event = None
        game_manager.board_manager.waiting_on_decision = False
    else:
        pov_player.calls_avaliable += [Call.Chi]
        game_manager.board_manager.waiting_on_decision = True


def on_pon_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    match,
    event_player,
    pov_player,
    _extra_player,
):
    if not is_decision:
        if is_ai:
            last_event = game_manager.board_manager.last_event
        else:
            last_event = game_manager.board_manager.last_decision_event
        del match.players[last_event.player].discard_pile[-1]

        if is_ai:
            del event_player.hand[-3:]
        else:
            event_player.hand.remove(event.piece)
            event_player.hand.remove(event.piece)

        match.current_board.current_turn = event.player
        event_player.melded_hand += [
            Meld(
                [event.piece] * 3,
                CallDirection.get_call_direction(
                    event_player.player_id, last_event.player
                ),
            )
        ]
        event_player.calls_avaliable = []

        game_manager.board_manager.last_decision_event = None
        game_manager.board_manager.waiting_on_decision = False
    else:
        pov_player.calls_avaliable += [Call.Pon]
        game_manager.board_manager.waiting_on_decision = True


def on_ron_event(
    game_manager,
    _is_ai,
    is_decision,
    event,
    _match,
    event_player,
    pov_player,
    _extra_player,
):
    if not is_decision:
        event_player.hand += [event.piece]
        event_player.calls_avaliable = []
        game_manager.board_manager.waiting_on_decision = False
        game_manager.board_manager.did_exhaustive_draw = False

    else:
        game_manager.board_manager.waiting_on_decision = True
        pov_player.calls_avaliable += [Call.Ron]


def on_converted_kan_event(
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
        game_manager.board_manager.last_decision_event = event
        event_player.calls_avaliable += [Call.Kan]
    else:
        # Converted Kan
        for melds in event_player.melded_hand:
            if melds.tiles == [event.piece] * 3:
                melds.tiles += [event.piece]
                melds.converted_kan = True
                break

        if is_ai:
            del event_player.hand[-1:]
        else:
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
        if is_ai:
            last_event = game_manager.board_manager.last_event
        else:
            last_event = game_manager.board_manager.last_decision_event
        del match.players[last_event.player].discard_pile[-1]

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
    event,
    _match,
    event_player,
    _pov_player,
    _extra_player,
):
    if is_decision:
        game_manager.board_manager.waiting_on_decision = True
        game_manager.board_manager.last_decision_event = event
        event_player.calls_avaliable += [Call.Tsumo]
    else:
        event_player.calls_avaliable = []
        game_manager.board_manager.waiting_on_decision = False
        game_manager.board_manager.did_exhaustive_draw = False


def on_riichi_event(
    game_manager,
    is_ai,
    is_decision,
    event,
    _match,
    event_player,
    pov_player,
    _extra_player,
):
    if is_decision:
        game_manager.board_manager.waiting_on_decision = True
        game_manager.board_manager.last_decision_event = event
        pov_player.calls_avaliable += [Call.Riichi]
    else:
        game_manager.board_manager.waiting_on_decision = False
        game_manager.board_manager.last_decision_event = None
        event_player.calls_avaliable = []
        event_player.riichi_declared = True
        if not is_ai:
            event_player.hand.remove(event.piece)
            event_player.hand.sort()

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
        game_manager.board_manager.waiting_on_decision = False
        match.play_clack()
        if not is_ai:
            if not event_player.riichi_declared:
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
        game_manager.board_manager.last_decision_event = event
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
    game_manager,
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
    game_manager.board_manager._point_diff_count = 0


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
    value = event.piece.get_raw_value() if isinstance(event.piece, Piece) else event.piece
    match.delta_scores[event.player] += value * 100
    game_manager.board_manager._point_diff_count += 1
    if game_manager.board_manager._point_diff_count >= 4:
        game_manager.board_manager.round_should_end = True


def on_end_event(
    game_manager,
    _is_ai,
    _is_decision,
    _event,
    match,
    _event_player,
    _pov_player,
    _extra_player,
):
    game_manager.board_manager.game_should_end = True
    match.match_alive = False


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
        if len(match.current_board.wall) > 0:
            del match.current_board.wall[-1]
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
        if len(match.current_board.wall) > 0:
            del match.current_board.wall[-1]
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
        if len(match.current_board.wall) > 0:
            del match.current_board.wall[-1]
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
    match,
    event_player,
    pov_player,
    extra_player,
):
    if is_decision:
        event_player.discard_pile += [event.piece]

        game_manager.board_manager.last_decision_event = event
        game_manager.board_manager.waiting_on_decision = True
        pov_player.calls_avaliable += [Call.Ron]
        if len(match.current_board.wall) > 0:
            del match.current_board.wall[-1]
    else:
        extra_player.hand += [event.piece]
        game_manager.board_manager.waiting_on_decision = False

def on_game_event(game_manager, event, match):
    is_ai = event.player != match.player_manager.player_id
    is_decision = event.decision
    event_type = event.type

    if (is_ai and event.player != -1 and event.type != EventType.PointDiff) or (not is_ai and event.type == EventType.Discard and is_decision):
        sleep(0.5)

    if (event.type == EventType.PointDiff):
        sleep(0.1)

    print('== NEW EVENT ==')
    print('Player:', event.player)
    print('Type:', event.type)
    print('Piece:', PieceType(event.piece.get_raw_value()) if isinstance(event.piece, Piece) else event.piece)
    print('Decision?', event.decision)

    EVENTS = {
        EventType.Chi: on_chi_event,
        EventType.Pon: on_pon_event,
        EventType.Ron: on_ron_event,
        EventType.ConvertedKan: on_converted_kan_event,
        EventType.ConcealedKan: on_concealed_kan_event,
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

    actual_player = match.players[match.player_manager.player_id]

    if event.player is not None or event.player in range(4):
        player = match.players[event.player]
    else:
        player = None

    if hasattr(event, "extra_player"):
        if event.extra_player is not None or event.extra_player in range(4):
            extra_player = match.players[event.extra_player]
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

    valid_dialogue_events = [
        EventType.Chi,
        EventType.Pon,
        EventType.Ron,
        EventType.ConvertedKan,
        EventType.ConcealedKan,
        EventType.Kan,
        EventType.Riichi
    ]

    if hasattr(game_manager.get_active_view(), "receive_ai_events"):
        if not is_decision:
            if event.player == game_manager.get_active_view().receive_ai_events and event.type in valid_dialogue_events:
                print('Registered Event', event.type)
                game_manager.get_active_view().watched_ai_event_log += [event]

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