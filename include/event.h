#pragma once
#include "pieces.h"

enum Events{
    Discard,
    Riichi,
    ConcealedKan,
    Decline,
    Chi,
    Pon,
    Ron,
    Kan,
    Tsumo
};

enum Players{
    East,
    South,
    West,
    North
};

struct Event {
    Players player;
    Piece piece;
    Events decision;
};