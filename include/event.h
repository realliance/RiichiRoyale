#pragma once
#include "piecetype.h"

enum EventType{
  Ron,
  Kan,
  Pon,
  Chi,
  Tsumo,
  ConcealedKan,
  Riichi,
  Discard,
  Decline,
  Dora,
  PointDiff,
  End
};

struct Event {
  EventType type;
  int player;
  int16_t piece;
  bool decision;
};

const Event END_EVENT = {End,0,0,false};