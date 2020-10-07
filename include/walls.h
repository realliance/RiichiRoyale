#pragma once
#include <deque>
#include <vector>
#include <string>
#include "pieces.h"

class Walls{
  std::deque<Piece> livingWalls;
  std::deque<Piece> deadWall;
  int doraCount = 1;
  int replacements = 4;
  public:
  explicit Walls();
  Piece TakePiece();
  std::vector<Piece> TakeHand(bool dealer);
  Piece TakeReplacementTile();
  std::vector<Piece> GetDoras() const;
  std::vector<Piece> GetUraDoras() const;
  int GetRemainingPieces() const;
};
