#include "event.h"
#include "piecetype.h"
#include <iostream>

auto operator<<(std::ostream& os, const Event& e) -> std::ostream&{
  os << "{type: " << EventTypeToStr(e.type);
  os << " player: " << e.player;
  uint8_t p = e.piece;
  os << " piece: " << Piece(p).toStr();
  os << " decision: " << e.decision << "}";
  return os;
}
