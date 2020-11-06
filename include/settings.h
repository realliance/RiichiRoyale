#pragma once
#include <array>
#include <string>
#include "mahjongns.h"
#include "piecetype.h"

namespace Mahjong {

  struct GameSettings {
    std::vector<std::string> seatControllers;
    std::vector<Piece> overrideWall;
    uint64_t seed = 0;
  };

}
