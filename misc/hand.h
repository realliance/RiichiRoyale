#pragma once
#include <vector>
#include "pieces.h"
#include "meld.h"

struct Hand {
  std::vector<Piece> pieces;
  std::vector<Meld> melds;
  bool closed = true;
};
