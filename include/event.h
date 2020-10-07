#pragma once
#include "pieces.h"
#include "enum.h"

struct Event {
    Wind player;
    Piece piece;
    Events type;
};