#include <fstream>

#include "analysis.h"
#include "handnode.h"
#include "walls.h"
#include "mahjongns.h"
using namespace Mahjong;

//assumption is 13 piece hand
// auto isTenpai(std::vector<const Node*> branch) -> bool {
//   int cnt = 0;
//   int8_t counts[256] = {};
//   std::vector<Piece> singles;
//   for(const auto& node : branch){
//     if(node->type == Node::Single){
//       cnt++;
//       singles.push_back(node->start);
//     }
//     counts[node->start.toUint8_t()]++;
//   }
//   if(cnt > 5 || cnt == 3){
//     return false;
//   }
//   if(cnt == 0){
//     return true;
//   }
//   if(cnt == 1){
//     if(counts[singles[0].toUint8_t()] < 4){
//       return true;
//     }
//     return false;
//   }
//   if(cnt == 2){
//     if(singles[0] == singles[1] && counts[singles[0].toUint8_t()] < 4){
//       return true;
//     }
//     if(singles[0]+1 == singles[1] && (counts[(singles[0]+2).toUint8_t()] < 4 || counts[(singles[0]-1).toUint8_t()] < 4)){
//       return true;
//     }
//     if(singles[0]+2 == singles[1] && counts[(singles[0]+1).toUint8_t()] < 4){
//       return true;
//     }
//     return false;
//   }
//   if(cnt == 4){
//     auto suit = singles[0].getSuit();
//     if(singles[1].getSuit() != suit || singles[2].getSuit() != suit ||singles[3].getSuit() != suit){
//       return false;
//     }
//     if(singles[0].getPieceNum()+3 != singles[1].getPieceNum()){
//       return false;
//     }
//     if(singles[1].getPieceNum()+2 != singles[2].getPieceNum()){
//       return false;
//     }
//     if(singles[2].getPieceNum()+1 != singles[3].getPieceNum()){
//       return false;
//     }
//     return true;
//   }
//   return false;
// }

const std::vector<Piece> PIECE_SET = {
    Piece::ONE_BAMBOO,Piece::TWO_BAMBOO,Piece::THREE_BAMBOO,Piece::FOUR_BAMBOO,
    Piece::FIVE_BAMBOO,Piece::SIX_BAMBOO,Piece::SEVEN_BAMBOO,Piece::EIGHT_BAMBOO,
    Piece::NINE_BAMBOO,Piece::ONE_PIN,Piece::TWO_PIN,Piece::THREE_PIN,Piece::FOUR_PIN,
    Piece::FIVE_PIN,Piece::SIX_PIN,Piece::SEVEN_PIN,Piece::EIGHT_PIN,Piece::NINE_PIN,
    Piece::ONE_CHARACTER,Piece::NINE_CHARACTER,Piece::TWO_CHARACTER,Piece::
    Piece::THREE_CHARACTER,Piece::FOUR_CHARACTER,Piece::FIVE_CHARACTER,
    Piece::SIX_CHARACTER,Piece::SEVEN_CHARACTER,Piece::EIGHT_CHARACTER,Piece::
    Piece::WHITE_DRAGON,Piece::GREEN_DRAGON,Piece::RED_DRAGON,Piece::EAST_WIND,
    Piece::SOUTH_WIND,Piece::NORTH_WIND,Piece::WEST_WIND
};

//assumption is 14 piece hand
auto inTenpai(std::vector<Piece> hand) -> bool {
  int8_t counts[256] = {};
  bool removedbefore[256] = {};
  for(const auto & p : hand){
    counts[p.toUint8_t()]++;
  }
  for(int i = 0; i < 14; i++){
    Piece removed = hand.front();
    hand.erase(hand.begin());
    if(removedbefore[removed.toUint8_t()]){
      hand.push_back(removed);
      continue;
    }
    removedbefore[removed.toUint8_t()] = true;
    for(const auto & p : PIECE_SET){
      if(counts[p.toUint8_t()] == 4 || p == removed){
        continue;
      }
      hand.push_back(p);
      Node* root = breakdownHand(hand);
      if(root->IsComplete()){
        return true;
      }
      hand.pop_back();
    }
    hand.push_back(removed);
  }
  return false;
}

int main(){
  std::map<int,int> cnt;
  std::map<int,int> oofscnt;
  for(int i = 0; i < 100000; i++){
    if(i % 10000 == 0){
      std::cout << i << std::endl;
    }
    std::vector<Piece> Hand = GetPossibleTenpaiHand(/*replacement:*/false);
    // auto walls = Walls();
    // auto Hand = walls.TakeHand();
    // Hand.push_back(walls.TakePiece());
    Node* root = breakdownHand(Hand);
    if(root->AsBranchVectors().size() > 1){
      int c = 15;
      for(const auto& branch : root->AsBranchVectors()){
        int cc = 0;
        for(const auto& node : branch){
          if(node->type == Node::Single){
            cc++;
          }
        }
        c = cc < c ? cc : c;
      }
      if(oofscnt.contains(c)){
        oofscnt[c]++;
      }else{
        oofscnt[c] = 1;
      }
      delete root;
      continue;
    }
    int c = 0;
    for(const auto& node : *root){
      if(node.type == Node::Single){
        c++;
      }
    }
    if(cnt.contains(c)){
      cnt[c]++;
    }else{
      cnt[c] = 1;
    }
    // if(c==5){
    //   std::ofstream os("hand.gv");
    //   root->DumpAsDot(os);
    //   os.close();
    //   delete root;
    //   return 0;
    // }
    if(!inTenpai(Hand)){
      std::ofstream os("hand.gv");
      root->DumpAsDot(os);
      os.close();
      delete root;
      return 1;
    }
    delete root;
  }
  for(const auto& [singleCnt,freq]: oofscnt){
    std::cout << "oofssingleCnt: " << singleCnt << ", freq:" << freq << std::endl;
  }
  for(const auto& [singleCnt,freq]: cnt){
    std::cout << "singleCnt: " << singleCnt << ", freq:" << freq << std::endl;
  }
  return 0;
}
