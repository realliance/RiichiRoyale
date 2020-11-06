#pragma once
#include <iosfwd>       // for ostream
#include <vector>       // for vector
#include "piecetype.h"  // for Piece

namespace Mahjong { 
  class Walls{
  public:
    std::vector<Piece> livingWalls;
    std::vector<Piece> deadWall;
    int doraCount = 1;
    int replacements = 4;
    explicit Walls();
    Piece TakePiece();
    std::vector<Piece> TakeHand();
    Piece TakeReplacementTile();
    std::vector<Piece> GetDoras() const;
    std::vector<Piece> GetUraDoras() const;
    int GetRemainingPieces() const;
  };
}
  
auto operator<<(std::ostream& os, const Mahjong::Walls& walls) -> std::ostream&;
