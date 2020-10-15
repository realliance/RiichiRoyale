#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>

#include "handnode.h"
#include "piecetype.h"
#include "walls.h"
#include "hand.h"

auto breakdownHand(std::vector<Piece> pieces) -> Node*;
