#include "player.h"
#include <string>       // for operator<<
#include "mahjongai.h"  // for MahjongAI

auto operator<<(std::ostream& os, const Player& player) -> std::ostream&{
  os << "{ controller: " << ((player.controller != nullptr) ? player.controller->Name() : "NULLPTR");
  os << " points: " << player.points;
  os << " position: " << player.position;
  os << " seat: " << WindToStr(player.seat) << "}";
  return os;
}
