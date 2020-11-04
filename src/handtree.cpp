#include <sys/types.h>
#include <algorithm>
#include <memory>
#include <vector>
#include "handnode.h"
#include "piecetype.h"
#include "analysis.h"


using namespace Mahjong;

int possibleChis(const int8_t* counts, Piece p){
  if(p.isHonor()){
    return 0;
  }
  if(counts[(p).toUint8_t()] > 3 && counts[(p+1).toUint8_t()] > 3 && counts[(p+2).toUint8_t()] > 3){
    return 4;
  }
  if(counts[(p).toUint8_t()] > 2 && counts[(p+1).toUint8_t()] > 2 && counts[(p+2).toUint8_t()] > 2){
    return 3;
  }
  if(counts[(p).toUint8_t()] > 1 && counts[(p+1).toUint8_t()] > 1 && counts[(p+2).toUint8_t()] > 1){
    return 2;
  }
  if(counts[(p).toUint8_t()] > 0 && counts[(p+1).toUint8_t()] > 0 && counts[(p+2).toUint8_t()] > 0){
    return 1;
  }
  return 0;
}

bool anyPossibleChi(const int8_t* counts, Piece p){
  return !p.isHonor() && (possibleChis(counts,p) > 0 || possibleChis(counts,p-1) > 0 || possibleChis(counts,p-2) > 0);
}

bool possiblePair(const int8_t* counts, Piece p){
  return counts[p.toUint8_t()] == 2 || (counts[p.toUint8_t()] == 3 && anyPossibleChi(counts,p) > 0);
}

bool possiblePon(const int8_t* counts, Piece p){
  return counts[p.toUint8_t()] == 3 || (counts[p.toUint8_t()] == 4 && anyPossibleChi(counts,p) > 0);
}

void countPieces(int8_t* counts, const std::vector<Piece>& pieces){
  for(const auto & p : pieces){
    counts[p.toUint8_t()]++;
  }
}

auto Mahjong::breakdownHand(std::vector<Piece> pieces) -> Node*{
  int8_t counts[256] = {};
  countPieces(counts,pieces);
  std::sort(pieces.begin(), pieces.end());
  pieces.erase(std::unique(pieces.begin(),pieces.end()),pieces.end());
  
  std::vector<Piece> pairs;
  std::vector<Piece> onlyPon;
  std::vector<Piece> onlyChi;

  bool changed = true;
  while(changed){
    changed = false;
    std::vector<Piece> temppairs;
    std::vector<Piece> temp;
    for(const auto & piece : pieces){
      if(counts[piece.toUint8_t()] == 0){
        continue;
      }
      if(possiblePair(counts,piece) && !anyPossibleChi(counts,piece) && !possiblePon(counts,piece)){
        temppairs.push_back(piece);
        continue;
      }
      if(!anyPossibleChi(counts,piece) && possiblePon(counts,piece)){
        onlyPon.push_back(piece);
        counts[piece.toUint8_t()] -= 3;
        continue;
      }
      if(possibleChis(counts,piece) > 0 && possibleChis(counts,piece-1) == 0 && possibleChis(counts,piece+1) == 0 && !possiblePon(counts,piece)){
        int cnt = possibleChis(counts,piece);
        onlyChi.resize(onlyChi.size() + cnt);
        std::generate_n(onlyChi.rbegin(),cnt,[piece](){return piece;});
        counts[piece.toUint8_t()] -= cnt;
        counts[(piece+1).toUint8_t()] -= cnt;
        counts[(piece+2).toUint8_t()] -= cnt;
        continue;
      }
    }
    if(temppairs.size() == 1){
      counts[temppairs[0].toUint8_t()] -= 2;
      pairs.push_back(temppairs[0]);
    }
  }

  for(const auto & piece : pieces){
    if(counts[piece.toUint8_t()] == 0){
      continue;
    }
    if(possibleChis(counts,piece) == 1 && !possiblePair(counts,piece) && !possiblePon(counts,piece)){
      onlyChi.push_back(piece);
      counts[piece.toUint8_t()] -= 1;
      counts[(piece+1).toUint8_t()] -= 1;
      counts[(piece+2).toUint8_t()] -= 1;
    }
    if(possiblePon(counts,piece) && !((possibleChis(counts,piece) == 1 && possibleChis(counts,piece+1) == 0 && possibleChis(counts,piece-1) == 0) && possiblePair(counts,piece))){
      onlyPon.push_back(piece);
      counts[piece.toUint8_t()] -= 3;
    }
    if(possibleChis(counts,piece) == 2){
      onlyChi.push_back(piece);
      onlyChi.push_back(piece);
      counts[piece.toUint8_t()] -= 2;
      counts[(piece+1).toUint8_t()] -= 2;
      counts[(piece+2).toUint8_t()] -= 2;
    }
    if(possiblePair(counts,piece)){
      pairs.push_back(piece);
      counts[piece.toUint8_t()] -= 2;
    }
    if(possibleChis(counts,piece) == 1){
      onlyChi.push_back(piece);
      counts[piece.toUint8_t()] -= 1;
      counts[(piece+1).toUint8_t()] -= 1;
      counts[(piece+2).toUint8_t()] -= 1;
    }
  }

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

  int singleCnt = 0;
  // add singles
  for(const auto & single : pieces){
    int n = 0;
    while(counts[single.toUint8_t()] > n++){
      Node* leaf = new Node{
        id++, //id
        Node::Single, //type
        single, //Start
        currentNode, //parent
        {}, // leaves
        currentNode->leaves.size() //leafPosInParent
      };
      singleCnt++;
      currentNode->leaves.push_back(leaf);
      currentNode = leaf;
    }
  }

  // add pons
  for(const auto & start : onlyPon){
    Node* leaf = new Node{
      id++, //id
      Node::PonSet, //type
      start, //Start
      currentNode, //parent
      {}, // leaves
      currentNode->leaves.size() //leafPosInParent
    };
    currentNode->leaves.push_back(leaf);
    currentNode = leaf;
  }

  // add chis
  for(const auto & start : onlyChi){
    Node* leaf = new Node{
      id++, //id
      Node::ChiSet, //type
      start, //Start
      currentNode, //parent
      {}, // leaves
      currentNode->leaves.size() //leafPosInParent
    };
    currentNode->leaves.push_back(leaf);
    currentNode = leaf;
  }

  // add possible pairs
  for(const auto & start : pairs){
    Node* leaf = new Node{
      id++, //id
      Node::Pair, //type
      start, //Start
      currentNode, //parent
      {}, // leaves
      currentNode->leaves.size() //leafPosInParent
    };
    currentNode->leaves.push_back(leaf);
  }

  //probably still left over pieces TODO

  // if(singleCnt > 0){
  //   std::cerr << "Failing Piece list: " << std::endl;
  //   for(const auto & single : pieces){
  //     if(counts[single.toUint8_t()] > 0){
  //       std::cerr << single.toStr() << ": " << std::to_string(counts[single.toUint8_t()]) << ", ";
  //     }
  //   }
  //   std::cerr << std::endl;
  //   std::ofstream os("hand.gv");
  //   rootNode->DumpAsDot(os);
  //   os.close();
  //   delete rootNode;
  //   return nullptr;
  // }

  return rootNode;
}
