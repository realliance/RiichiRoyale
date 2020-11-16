#include <sys/types.h>
#include <algorithm>
#include <memory>
#include <vector>
#include "handnode.h"
#include "piecetype.h"
#include "analysis.h"


#include "mahjongns.h"
using namespace Mahjong;

bool possibleChiForward(const int8_t* counts, Piece p){
  if(p.isHonor()){
    return false;
  }
  // if(counts[(p).toUint8_t()] == 3 && counts[(p).toUint8_t()] == 3 && counts[(p).toUint8_t()] == 2 && counts[(p+3).toUint8_t()] == 0)
  // if(counts[(p-1).toUint8_t()] == 0 && counts[(p).toUint8_t()] == 2 && counts[(p+1).toUint8_t()] == 1){
  //   return false;
  // }
  // if(counts[(p).toUint8_t()] == 1 && counts[(p+1).toUint8_t()] == 1 && counts[(p+2).toUint8_t()] == 2 && counts[(p+3).toUint8_t()] == 0){
  //   return false;
  // }
  // if(counts[(p).toUint8_t()] == 3 && counts[(p+1).toUint8_t()] == 1 && counts[(p+2).toUint8_t()] == 3 && counts[(p+3).toUint8_t()] > 0){
  //   return false;
  // }
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
  return (counts[p.toUint8_t()] == 2);// || (counts[p.toUint8_t()] == 3 && !p.isHonor() && anyPossibleChi(counts,p));
}

bool possiblePon(const int8_t* counts, Piece p){
  return counts[p.toUint8_t()] == 3;
}

void countPieces(int8_t* counts, const std::vector<Piece>& pieces){
  for(const auto & p : pieces){
    counts[p.toUint8_t()]++;
  }
}

int updatePossibilities(std::vector<int>& possibilities, const std::vector<Piece>& pieces, const int8_t* counts, bool paired){
  possibilities.resize(pieces.size());
  int minPossible = 6;
  for(size_t i = 0; i < pieces.size(); i++){
    possibilities[i] = 0;
    possibilities[i] += possibleChis(counts,pieces[i]);
    possibilities[i] += paired ? 0 : possiblePair(counts,pieces[i]);
    possibilities[i] += possiblePon(counts,pieces[i]);
    minPossible = possibilities[i] < minPossible ? possibilities[i] : minPossible;
  }
  return minPossible;
}


auto Mahjong::breakdownHand(std::vector<Piece> pieces) -> Node*{
  int8_t counts[256] = {};
  countPieces(counts,pieces);
  std::sort(pieces.begin(), pieces.end());
  pieces.erase(std::unique(pieces.begin(),pieces.end()),pieces.end());
  std::cout << "Pieces: ";
  for(const auto p : pieces){
    std::cout << p.toStr() << ' ';
  }
  std::cout << std::endl;
  
  std::vector<int> possibilities;
  int minPossible = updatePossibilities(possibilities,pieces,counts, false);

  int id = 0;
  Node* rootNode = new Node{
    id++, //id
    Node::Root, //type
    Piece::ERROR, //Start
    nullptr, //parent
    {}, // leaves
    0 //leafPosInParent
  };

  Node* currentNode = rootNode;
  bool paired = false;

  while(pieces.size() > 0){
    int piecePos = 0;
    for(size_t i = 0; i < pieces.size(); i++){
      if(possibilities[i] <= possibilities[piecePos]){
        piecePos = i;
        if(possibilities[i] == minPossible){
          break;
        }
      }
    }
    if(possibilities[piecePos] == 0){
      Node* leaf = new Node{
        id++, //id
        Node::Single, //type
        pieces[piecePos], //Start
        currentNode, //parent
        {}, // leaves
        currentNode->leaves.size() //leafPosInParent
      };
      currentNode->leaves.push_back(leaf);
      currentNode = leaf;
      counts[pieces[piecePos].toUint8_t()] = 0;
      pieces.erase(std::remove(pieces.begin(), pieces.end(), pieces[piecePos]), pieces.end());
    } else if(possibilities[piecePos] == 1){
      Piece start = Piece::ERROR;
      Node::Type type = Node::Error;
      if(anyPossibleChi(counts,pieces[piecePos])){
        for(int i = 0; i < 3; i++){
          if(possibleChiForward(counts,pieces[piecePos-i])){
            start = pieces[piecePos-i];
            int k = 0;
            for(int j = 0; j < 3; j++){
              counts[pieces[piecePos-i+k].toUint8_t()]--;
              std::cout << "Count: " << int(counts[pieces[piecePos-i+k].toUint8_t()]) << " Piece: " << pieces[piecePos-i+k].toStr() << std::endl;
              if(counts[pieces[piecePos-i+k].toUint8_t()] == 0){
                std::cout << "Removed: at i: " << i << " j: " << j << "Piece: " << pieces[piecePos-i+k].toStr() << std::endl; 
                pieces.erase(std::remove(pieces.begin(), pieces.end(), pieces[piecePos-i+k]), pieces.end());
                k--;
              }
              k++;
            }
            piecePos = piecePos-i;
            type = Node::ChiSet;
            break;
          }
        }
      }else if(possiblePon(counts,pieces[piecePos])){
        start = pieces[piecePos];
        type = Node::PonSet;
        std::cout << pieces[piecePos].toStr() << std::endl;
        std::cout << int(counts[pieces[piecePos].toUint8_t()]) << std::endl;
        counts[pieces[piecePos].toUint8_t()]-=3;
        if(counts[pieces[piecePos].toUint8_t()] == 0){
          pieces.erase(std::remove(pieces.begin(), pieces.end(), pieces[piecePos]), pieces.end());
        }
      }else if(possiblePair(counts,pieces[piecePos])){
        paired = true;
        type = Node::Pair;
        start = pieces[piecePos];
        counts[pieces[piecePos].toUint8_t()]-=2;
        if(counts[pieces[piecePos].toUint8_t()] == 0){
          pieces.erase(std::remove(pieces.begin(), pieces.end(), pieces[piecePos]), pieces.end());
        }
      }
      Node* leaf = new Node{
        id++, //id
        type, //type
        start, //Start
        currentNode, //parent
        {}, // leaves
        currentNode->leaves.size() //leafPosInParent
      };
      currentNode->leaves.push_back(leaf);
      currentNode = leaf;
    } else if(possibilities[piecePos] > 1){
      std::cout << "poss: " << int(possibilities[piecePos]) << " piece: " << pieces[piecePos].toStr() << std::endl;
      std::cout << "Pieces: ";
      for(const auto p : pieces){
        std::cout << p.toStr() << ' ';
      }
      std::cout << std::endl;
      std::cout << "oops" << std::endl;
      break;
    }
    minPossible = updatePossibilities(possibilities,pieces,counts, paired);
  }


  return rootNode;
}
