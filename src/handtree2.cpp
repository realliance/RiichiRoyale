#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>

#include "handnode.h"
#include "piecetype.h"
#include "walls.h"
#include "hand.h"

void countPieces(int8_t* counts, const std::vector<Piece>& pieces){
  for(const auto & p : pieces){
    counts[p.toUint8_t()]++;
  }
}


std::vector<Node*> treeParts(int8_t* counts, std::vector<Piece> liveHand){
  std::vector<Node*> parts;
  std::sort(liveHand.begin(),liveHand.end());
  int id = 0;
  Node* rootNode = new Node{
    id++, //id
    Root, //type
    ERROR_PIECE, //Start
    nullptr, //parent
    {}, // leaves
    0 //leafPosInParent
  };
  Node* pin;
  Node* bamboo;
  Node* character;
  for(auto const & piece : liveHand){
    
  }
}