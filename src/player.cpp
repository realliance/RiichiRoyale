#include "player.h"
#include <string>       // for operator<<
#include "mahjongai.h"  // for MahjongAI

auto operator<<(std::ostream& os, const Player& player) -> std::ostream&{
  os << "{ controller: " << player.controller->Name();
  os << " points: " << player.points;
  os << " position: " << player.position;
  os << " seat: " << WindToStr(player.seat) << "}";
  return os;
}
