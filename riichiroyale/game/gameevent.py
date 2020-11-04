from enum import IntEnum
from libmahjong import EventType


class CombinedEventType(IntEnum):
    DiscardRon = 100 + int(EventType.Ron)
    DiscardKan = 100 + int(EventType.Kan)
    DiscardPon = 100 + int(EventType.Pon)
    DiscardChi = 100 + int(EventType.Chi)


class Event:
    def __init__(
        self,
        event_type,
        player,
        piece,
        decision,
        extra_player=None,
        extra_piece=None,
        raw_event_a=None,
        raw_event_b=None,
    ):
        self.type = event_type
        self.player = player
        self.piece = piece
        self.extra_player = extra_player
        self.extra_piece = extra_piece
        self.decision = decision
        self.raw_event_a = raw_event_a
        self.raw_event_b = raw_event_b
