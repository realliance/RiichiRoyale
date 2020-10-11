#pragma once
#include "pieces.h"
#include "enum.h"

struct Event {
    Wind player;
    EventType type;
    Piece piece;
};
