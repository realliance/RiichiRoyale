#include "meld.h"
#include <ostream>  // for operator<<, basic_ostream

auto operator<<(std::ostream& os, const Meld& meld) -> std::ostream&{
  os << "{ type: " << MeldTypeToStr(meld.type);
  os << ", pieces: [";
  for(const auto & piece : meld.pieces){
    os << piece.toStr() << ", ";
  }
  os << "] }";
  return os;
}
