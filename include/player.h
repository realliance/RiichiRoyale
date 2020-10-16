#pragma once
#include "hand.h"

class MahjongAI;

struct Player {
  int points;
  int position;
  Wind seat;
  MahjongAI* controller;
};
