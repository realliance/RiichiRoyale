#include <ostream>  // for operator<<, basic_ostream
#include "meld.h"


using namespace Mahjong;

auto operator<<(std::ostream& os, const Meld& meld) -> std::ostream&{
  os << "{ type: " << MeldTypeToStr(meld.type);
  os << ", start: " << meld.start.toStr() << "}";
  return os;
}
