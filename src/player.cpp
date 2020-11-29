#include "player.h"
#include <string>
#include "playercontroller.h"

using namespace Mahjong;

auto operator<<(std::ostream& os, const Player& player) -> std::ostream&{
  os << "{ controller: " << ((player.controller != nullptr) ? player.controller->Name() : "NULLPTR");
  os << " points: " << player.points;
  return os;
}
