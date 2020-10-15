#include <algorithm>
#include <fstream>
#include <iostream>

#include "handnode.h"
#include "piecetype.h"
#include "walls.h"
#include "hand.h"



bool possibleChi(uint8_t* counts, Piece p){
  if(p.isHonor()){
    return false;
  }
  return counts[p.value()] > 0 && counts[p.value()+1] > 0 && counts[p.value()+2] > 0;
}

bool possiblePair(uint8_t* counts, Piece p){
  return counts[p.value()] >= 2;
}

bool possiblePon(uint8_t* counts, Piece p){
  return counts[p.value()] >= 3;
}

void countPieces(uint8_t* counts, const std::vector<Piece>& pieces){
  for(const auto & p : pieces){
    counts[p.value()]++;
  }
}

void createNodes(uint8_t* counts, const std::vector<Piece>& pieces, std::vector<Piece>& pairs, std::vector<Piece>& pon, std::vector<Piece>& chi, std::vector<Piece>& singles){
  for(const auto & piece : pieces){
    bool match = false;
    if(possibleChi(counts,piece)){
      match = true;
      chi.push_back(piece);
    }
    if(possiblePair(counts,piece)){
      match = true;
      pairs.push_back(piece);
    }
    if(possiblePon(counts,piece)){
      match = true;
      pon.push_back(piece);
    }
    if(!match){
      singles.push_back(piece);
    }
  }
}

auto breakdownHand(std::vector<Piece> pieces) -> Node*{
  uint8_t counts[256] = {};
  countPieces(counts,pieces);
  std::sort(pieces.begin(), pieces.end());
  std::unique(pieces.begin(),pieces.end());
  
  std::vector<Piece> pairs;
  std::vector<Piece> pon;
  std::vector<Piece> chi;
  std::vector<Piece> singles;
  createNodes(counts,pieces,pairs,pon,chi,singles);

  int id = 0;
  Node* rootNode = new Node{
    id++, //id
    Root, //type
    ERROR_PIECE, //Start
    nullptr, //parent
    {}, // leaves
    0 //leafPosInParent
  };

  Node* currentNode = rootNode;

  for(const auto & single : singles){
    Node* leaf = new Node{
      id++, //id
      Single, //type
      single, //Start
      currentNode, //parent
      {}, // leaves
      currentNode->leaves.size() //leafPosInParent
    };
    currentNode->leaves.push_back(leaf);
    currentNode = leaf;
  }

  for(const auto & pair : pairs){
    Node* leaf = new Node{
      id++, //id
      Pair, //type
      pair, //Start
      currentNode, //parent
      {}, // leaves
      currentNode->leaves.size() //leafPosInParent
    };
    currentNode->leaves.push_back(leaf);
  }

  return rootNode;
}
