#include "meld.h"
#include <ostream>  // for operator<<, basic_ostream

auto operator<<(std::ostream& os, const Meld& meld) -> std::ostream&{
  os << "{ type: " << MeldTypeToStr(meld.type);
  os << ", start: " << meld.start << "}";
  return os;
}
