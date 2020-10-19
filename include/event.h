#pragma once
#include <stdint.h>  // for int16_t
#include <iostream>  // for ostream
#include <string>    // for allocator, string

//Do not rearrange -Alice
enum EventType{
  // out of hand events
  Ron,
  Kan,
  Pon,
  Chi,
  Decline,
  // in hand events
  Tsumo,
  ConcealedKan,
  ConvertedKan,
  Riichi,
  Discard,
  // other game events
  Dora,
  PointDiff,
  ExhaustiveDraw,
  End
};

struct Event {
  EventType type;
  int player;
  int16_t piece;
  bool decision;
};
auto operator<<(std::ostream& os, const Event& e) -> std::ostream&;

const Event END_EVENT = {End,0,0,false};
const Event DECLINE_EVENT = {Decline,0,0,false};

inline auto EventTypeToStr(EventType s) -> std::string{
  switch(s){
    case Ron:
      return "Ron";
    case Kan:
      return "Kan";
    case Pon:
      return "Pon";
    case Chi:
      return "Chi";
    case Tsumo:
      return "Tsumo";
    case ConcealedKan:
      return "ConcealedKan";
    case ConvertedKan:
      return "ConvertedKan";
    case Riichi:
      return "Riichi";
    case Discard:
      return "Discard";
    case Decline:
      return "Decline";
    case Dora:
      return "Dora";
    case PointDiff:
      return "PointDiff";
    case ExhaustiveDraw:
      return "ExhaustiveDraw";
    case End:
      return "End";
    default:
      return "InvalidState";

  }
}
