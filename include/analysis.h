#pragma once
#include <vector>  // for vector

#include "handnode.h"
#include "piecetype.h"

namespace Mahjong{

  auto GetPossibleStdFormHand() -> std::vector<Piece>;
  auto GetPossibleTenpaiHand(bool replacement = false) -> std::vector<Piece>;
  auto TestStdForm(std::vector<Piece> hand) -> bool;
  auto breakdownHand(std::vector<Piece> pieces) -> Node*;

}
