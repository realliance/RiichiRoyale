#pragma once
#include "pieces.h"
#include "enum.h"

struct Event {
  EventType type;
  Wind player;
  Piece piece;
};
