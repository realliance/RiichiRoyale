#pragma once
#include <vector>
#include "pieces.h"
#include "hand.h"
#include "walls.h"
#include "enum.h"

struct Board{
  Walls walls;
  std::vector<Hand> hands;
  Wind wind;
};
