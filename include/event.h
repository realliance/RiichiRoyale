#pragma once
#include "piecetype.h"
#include "enum.h"

struct Event {
  EventType type;
  int player;
  Piece piece;
};
