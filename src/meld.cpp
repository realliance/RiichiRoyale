#include <ostream>
#include "meld.h"


#include "mahjongns.h"
using namespace Mahjong;

auto operator<<(std::ostream& os, const Meld& meld) -> std::ostream&{
  os << "{ type: " << MeldTypeToStr(meld.type);
  os << ", start: " << meld.start.toStr() << "}";
  return os;
}
