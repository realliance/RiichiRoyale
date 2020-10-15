#pragma once
#include "enum.h"
#include "hand.h"

class MahjongAI;

struct Player {
  int points;
  int position;
  Wind seat;
  Hand hand;
  MahjongAI* controller;
};
