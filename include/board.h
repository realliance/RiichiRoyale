#pragma once
#include <vector>
#include "pieces.h"
#include "hand.h"

struct Board{
  std::vector<Hand> hands;
  std::vector<Piece> Dora;
  Winds Wind;
};
