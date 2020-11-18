#include <sys/types.h>
#include <algorithm>
#include <memory>
#include <iostream>
 #include <fstream>
#include <vector>
#include "handnode.h"
#include "piecetype.h"
#include "analysis.h"


#include "mahjongns.h"
using namespace Mahjong;

struct Breakdown {
  Node* rootNode;
  Node* currentNode;
  bool paired = false;
  int minPossible;
  int id = 0;
  int8_t counts[256] = {};
  std::vector<int> possibilities;
  std::vector<Piece> pieces;
};

bool possibleChiForward(const int8_t* counts, Piece p){
  if(p.isHonor()){
    return false;
  }
  if(counts[(p).toUint8_t()] > 0 && counts[(p+1).toUint8_t()] > 0 && counts[(p+2).toUint8_t()] > 0){
    return true;
  }
  return false;
}

//this probably works well enough
int possibleChis(const int8_t* counts, Piece p){
  return p.isHonor() ? 0 : ((possibleChiForward(counts,p) + possibleChiForward(counts,p-1) + possibleChiForward(counts,p-2)));
}

bool anyPossibleChi(const int8_t* counts, Piece p){
  return possibleChis(counts,p) > 0;
}

bool possiblePair(const int8_t* counts, Piece p){
  return (counts[p.toUint8_t()] == 2);
}

bool possiblePon(const int8_t* counts, Piece p){
  return counts[p.toUint8_t()] == 3;
}

void countPieces(Breakdown& b){
  for(const auto & p : b.pieces){
    b.counts[p.toUint8_t()]++;
  }
}

void updatePossibilities(Breakdown& b){
  b.possibilities.resize(b.pieces.size());
  b.minPossible = 6;
  for(size_t i = 0; i < b.pieces.size(); i++){
    b.possibilities[i] = 0;
    b.possibilities[i] += possibleChis(b.counts,b.pieces[i]);
    b.possibilities[i] += b.paired ? 0 : possiblePair(b.counts,b.pieces[i]);
    b.possibilities[i] += possiblePon(b.counts,b.pieces[i]);
    b.minPossible = b.possibilities[i] < b.minPossible ? b.possibilities[i] : b.minPossible;
  }
}

auto addLeaf(Breakdown& b, Piece start, Node::Type type) -> Node* {
  Node* leaf = new Node{
    b.id++, //id
    type, //type
    start, //Start
    b.currentNode, //parent
    {}, // leaves
    b.currentNode->leaves.size() //leafPosInParent
  };
  b.currentNode->leaves.push_back(leaf);
  return leaf;
}

auto breakdownForwardChi(Breakdown& b, int piecePos) -> void {
  Piece start = b.pieces[piecePos];
  for(int i = 0; i < 3; i++){
    b.counts[(start+i).toUint8_t()]--;
    if(b.counts[(start+i).toUint8_t()] == 0){
      b.pieces.erase(std::remove(b.pieces.begin(), b.pieces.end(), (start+i)), b.pieces.end());
    }
  }
  b.currentNode = addLeaf(b, start, Node::ChiSet);
}

auto breakdownPon(Breakdown& b, int piecePos) -> void {
  Piece start = b.pieces[piecePos];
  b.counts[b.pieces[piecePos].toUint8_t()]-=3;
  if(b.counts[b.pieces[piecePos].toUint8_t()] == 0){
    b.pieces.erase(std::remove(b.pieces.begin(), b.pieces.end(), b.pieces[piecePos]), b.pieces.end());
  }
  b.currentNode = addLeaf(b, start, Node::PonSet);;
}

auto breakdownPair(Breakdown& b, int piecePos) -> void {
  b.paired = true;
  Piece start = b.pieces[piecePos];
  b.counts[b.pieces[piecePos].toUint8_t()]-=2;
  if(b.counts[b.pieces[piecePos].toUint8_t()] == 0){
    b.pieces.erase(std::remove(b.pieces.begin(), b.pieces.end(), b.pieces[piecePos]), b.pieces.end());
  }
  b.currentNode = addLeaf(b, start, Node::Pair);;
}

auto breakdownSingle(Breakdown& b, int piecePos) -> void {
  b.currentNode = addLeaf(b, b.pieces[piecePos], Node::Single);;
  b.counts[b.pieces[piecePos].toUint8_t()]--;
  if(b.counts[b.pieces[piecePos].toUint8_t()] == 0){
    b.pieces.erase(std::remove(b.pieces.begin(), b.pieces.end(), b.pieces[piecePos]), b.pieces.end());
  }
}

auto getNextPiece(Breakdown& b) -> int {
  int piecePos = 0;
  for(size_t i = 0; i < b.pieces.size(); i++){
    if(b.possibilities[i] <= b.possibilities[piecePos]){
      piecePos = i;
      if(b.possibilities[i] == b.minPossible){
        break;
      }
    }
  }
  return piecePos;
}

auto resetCounts(Breakdown& b, Node* target) -> void {
  if(!b.currentNode){
    std::cerr << "reset Failure: current node nullptr." << std::endl;
    std::ofstream os("error.gv");
    b.rootNode->DumpAsDot(os);
    os.close();
    throw -1;
  }
  while(b.currentNode != target){
    if(!b.currentNode->parent){
      std::cerr << "reset Failure: parent node nullptr." << std::endl;
      std::ofstream os("error.gv");
      b.rootNode->DumpAsDot(os);
      os.close();
      throw -2;
    }
    if(b.currentNode->parent->type == Node::Error){
      std::cerr << "reset Failure: reset up to an error." << std::endl;
      std::ofstream os("error.gv");
      b.rootNode->DumpAsDot(os);
      os.close();
      throw -4;
    }
    if(b.currentNode->type == Node::ChiSet){
      for(int i = 0; i < 3; i++){
        if(b.counts[(b.currentNode->start+i).toUint8_t()] == 0){
          b.pieces.push_back(Piece{b.currentNode->start+i});
          std::sort(b.pieces.begin(), b.pieces.end());
        }
        b.counts[(b.currentNode->start+i).toUint8_t()]++;
      }
    }else{
      if(b.counts[b.currentNode->start.toUint8_t()] == 0){
        b.pieces.push_back(Piece{b.currentNode->start});
        std::sort(b.pieces.begin(), b.pieces.end());
      }
      if(b.currentNode->type == Node::Single){
        b.counts[b.currentNode->start.toUint8_t()]++;
      }
      if(b.currentNode->type == Node::Pair){
        b.paired = false;
        b.counts[b.currentNode->start.toUint8_t()]+=2;
      }
      if(b.currentNode->type == Node::PonSet){
        b.counts[b.currentNode->start.toUint8_t()]+=3;
      }
    }
    b.currentNode = b.currentNode->parent;
  }
}

auto driver(Breakdown& b) -> void {
  updatePossibilities(b);
  if(b.pieces.empty()){
    return;
  }
  int piecePos = getNextPiece(b);
  if(b.possibilities[piecePos] == 0){
    breakdownSingle(b, piecePos);
    return driver(b);
  }
  if(b.possibilities[piecePos] == 1){
    if(anyPossibleChi(b.counts,b.pieces[piecePos])){
      for(int i = 0; i < 3; i++){
        if(possibleChiForward(b.counts,b.pieces[piecePos]-i)){
          breakdownForwardChi(b,piecePos-i);
          break;
        }
      }
      return driver(b);
    }
    if(possiblePon(b.counts,b.pieces[piecePos])){
      breakdownPon(b, piecePos);
      return driver(b);
    }
    if(possiblePair(b.counts,b.pieces[piecePos])){
      breakdownPair(b, piecePos);
      return driver(b);
    }
  }
  if(b.possibilities[piecePos] == 2){
    Node* current = b.currentNode;
    int branch = 0;
    if(possibleChiForward(b.counts,b.pieces[piecePos]-0)){
      branch++;
      breakdownForwardChi(b,piecePos-0);
      driver(b);
      resetCounts(b, current);
    }
    if(possibleChiForward(b.counts,b.pieces[piecePos]-1)){
      branch++;
      breakdownForwardChi(b,piecePos-1);
      if(branch == 2){
        return driver(b);
      }
      driver(b);
      resetCounts(b, current);
    }
    if(possibleChiForward(b.counts,b.pieces[piecePos]-2)){
      branch++;
      breakdownForwardChi(b,piecePos-2);
      if(branch == 2){
        return driver(b);
      }
      driver(b);
      resetCounts(b, current);
    }
    if(possiblePon(b.counts,b.pieces[piecePos])){
      branch++;
      breakdownPon(b, piecePos);
      if(branch == 2){
        return driver(b);
      }
      driver(b);
      resetCounts(b, current);
    }
    if(possiblePair(b.counts,b.pieces[piecePos])){
      branch++;
      breakdownPair(b, piecePos);
      if(branch == 2){
        return driver(b);
      }
      driver(b);
    }
    std::cerr << "possibilities Failure: less than two branches taken." << std::endl;
    std::ofstream os("error.gv");
    b.rootNode->DumpAsDot(os);
    os.close();
    throw -5;
    return;
  }
  if(b.possibilities[piecePos] == 3){
    throw -1;
    return;
  }
}

auto Mahjong::breakdownHand(std::vector<Piece> _pieces) -> Node*{
  Breakdown b;
  b.pieces = _pieces;
  countPieces(b);
  std::sort(b.pieces.begin(), b.pieces.end());
  b.pieces.erase(std::unique(b.pieces.begin(),b.pieces.end()),b.pieces.end());

  b.rootNode = new Node{
    b.id++, //id
    Node::Root, //type
    Piece::ERROR, //Start
    nullptr, //parent
    {}, // leaves
    0 //leafPosInParent
  };
  b.currentNode = b.rootNode;

  driver(b);

  return b.rootNode;
}
