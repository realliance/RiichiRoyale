#pragma once
#include <vector>  // for vector

#include "handnode.h"
#include "piecetype.h"

namespace Mahjong{

  auto GetPossibleStdFormHand() -> std::vector<Piece>;

  auto breakdownHand(std::vector<Piece> pieces) -> Node*;

}
