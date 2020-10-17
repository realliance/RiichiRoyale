#pragma once
#include <deque>        // for deque
#include <iosfwd>       // for ostream
#include <vector>       // for vector
#include "piecetype.h"  // for Piece

class Walls{
  std::deque<Piece> livingWalls;
  std::deque<Piece> deadWall;
  int doraCount = 1;
  int replacements = 4;
  friend auto operator<<(std::ostream& os, const Walls& walls) -> std::ostream&;
  public:
  explicit Walls();
  Piece TakePiece();
  std::vector<Piece> TakeHand();
  Piece TakeReplacementTile();
  std::vector<Piece> GetDoras() const;
  std::vector<Piece> GetUraDoras() const;
  int GetRemainingPieces() const;
};

auto operator<<(std::ostream& os, const Walls& walls) -> std::ostream&;
