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
  static std::string path;
  public:
  explicit Walls();
  Piece TakePiece();
  std::vector<Piece> TakeHand();
  Piece TakeReplacementTile();
  std::vector<Piece> GetDoras() const;
  std::vector<Piece> GetUraDoras() const;
  int GetRemainingPieces() const;
  static std::string GetPath(Piece piece);
  static void SetPath(std::string filePath);
  static void Sort(std::vector<Piece>& pieces);
};
