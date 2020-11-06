#include "analysis.h"

#include <stdint.h>            // for uint8_t
#include <algorithm>           // for shuffle,Piece::move
#include <iterator>            // for back_insert_iterator,Piece::back_inserter
#include <memory>              // for allocator_traits<>::value_type
#include <random>              // for mt19937,Piece::uniform_int_distribution,Piece::ran...
#include <ext/alloc_traits.h>  // for __alloc_traits<>::value_type
#include <vector>              // for vector

#include "piecetype.h"         // for Piece,Piece::FIVE_BAMBOO,Piece::FIVE_CHARACTER

#include "mahjongns.h"
using namespace Mahjong;

const std::vector<std::vector<Piece>> TRIPLETS = {
  {Piece::ONE_BAMBOO,Piece::TWO_BAMBOO,Piece::THREE_BAMBOO},
  {Piece::TWO_BAMBOO,Piece::THREE_BAMBOO,Piece::FOUR_BAMBOO},
  {Piece::THREE_BAMBOO,Piece::FOUR_BAMBOO,Piece::FIVE_BAMBOO},
  {Piece::FOUR_BAMBOO,Piece::FIVE_BAMBOO,Piece::SIX_BAMBOO},
  {Piece::FIVE_BAMBOO,Piece::SIX_BAMBOO,Piece::SEVEN_BAMBOO},
  {Piece::SIX_BAMBOO,Piece::SEVEN_BAMBOO,Piece::EIGHT_BAMBOO},
  {Piece::SEVEN_BAMBOO,Piece::EIGHT_BAMBOO,Piece::NINE_BAMBOO},
  {Piece::ONE_CHARACTER,Piece::TWO_CHARACTER,Piece::THREE_CHARACTER},
  {Piece::TWO_CHARACTER,Piece::THREE_CHARACTER,Piece::FOUR_CHARACTER},
  {Piece::THREE_CHARACTER,Piece::FOUR_CHARACTER,Piece::FIVE_CHARACTER},
  {Piece::FOUR_CHARACTER,Piece::FIVE_CHARACTER,Piece::SIX_CHARACTER},
  {Piece::FIVE_CHARACTER,Piece::SIX_CHARACTER,Piece::SEVEN_CHARACTER},
  {Piece::SIX_CHARACTER,Piece::SEVEN_CHARACTER,Piece::EIGHT_CHARACTER},
  {Piece::SEVEN_CHARACTER,Piece::EIGHT_CHARACTER,Piece::NINE_CHARACTER},
  {Piece::ONE_PIN,Piece::TWO_PIN,Piece::THREE_PIN},
  {Piece::TWO_PIN,Piece::THREE_PIN,Piece::FOUR_PIN},
  {Piece::THREE_PIN,Piece::FOUR_PIN,Piece::FIVE_PIN},
  {Piece::FOUR_PIN,Piece::FIVE_PIN,Piece::SIX_PIN},
  {Piece::FIVE_PIN,Piece::SIX_PIN,Piece::SEVEN_PIN},
  {Piece::SIX_PIN,Piece::SEVEN_PIN,Piece::EIGHT_PIN},
  {Piece::SEVEN_PIN,Piece::EIGHT_PIN,Piece::NINE_PIN},
  {Piece::ONE_BAMBOO,Piece::ONE_BAMBOO,Piece::ONE_BAMBOO},
  {Piece::TWO_BAMBOO,Piece::TWO_BAMBOO,Piece::TWO_BAMBOO},
  {Piece::THREE_BAMBOO,Piece::THREE_BAMBOO,Piece::THREE_BAMBOO},
  {Piece::FOUR_BAMBOO,Piece::FOUR_BAMBOO,Piece::FOUR_BAMBOO},
  {Piece::FIVE_BAMBOO,Piece::FIVE_BAMBOO,Piece::FIVE_BAMBOO},
  {Piece::SIX_BAMBOO,Piece::SIX_BAMBOO,Piece::SIX_BAMBOO},
  {Piece::SEVEN_BAMBOO,Piece::SEVEN_BAMBOO,Piece::SEVEN_BAMBOO},
  {Piece::EIGHT_BAMBOO,Piece::EIGHT_BAMBOO,Piece::EIGHT_BAMBOO},
  {Piece::NINE_BAMBOO,Piece::NINE_BAMBOO,Piece::NINE_BAMBOO},
  {Piece::ONE_CHARACTER,Piece::ONE_CHARACTER,Piece::ONE_CHARACTER},
  {Piece::TWO_CHARACTER,Piece::TWO_CHARACTER,Piece::TWO_CHARACTER},
  {Piece::THREE_CHARACTER,Piece::THREE_CHARACTER,Piece::THREE_CHARACTER},
  {Piece::FOUR_CHARACTER,Piece::FOUR_CHARACTER,Piece::FOUR_CHARACTER},
  {Piece::FIVE_CHARACTER,Piece::FIVE_CHARACTER,Piece::FIVE_CHARACTER},
  {Piece::SIX_CHARACTER,Piece::SIX_CHARACTER,Piece::SIX_CHARACTER},
  {Piece::SEVEN_CHARACTER,Piece::SEVEN_CHARACTER,Piece::SEVEN_CHARACTER},
  {Piece::EIGHT_CHARACTER,Piece::EIGHT_CHARACTER,Piece::EIGHT_CHARACTER},
  {Piece::NINE_CHARACTER,Piece::NINE_CHARACTER,Piece::NINE_CHARACTER},
  {Piece::ONE_PIN,Piece::ONE_PIN,Piece::ONE_PIN},
  {Piece::TWO_PIN,Piece::TWO_PIN,Piece::TWO_PIN},
  {Piece::THREE_PIN,Piece::THREE_PIN,Piece::THREE_PIN},
  {Piece::FOUR_PIN,Piece::FOUR_PIN,Piece::FOUR_PIN},
  {Piece::FIVE_PIN,Piece::FIVE_PIN,Piece::FIVE_PIN},
  {Piece::SIX_PIN,Piece::SIX_PIN,Piece::SIX_PIN},
  {Piece::SEVEN_PIN,Piece::SEVEN_PIN,Piece::SEVEN_PIN},
  {Piece::EIGHT_PIN,Piece::EIGHT_PIN,Piece::EIGHT_PIN},
  {Piece::NINE_PIN,Piece::NINE_PIN,Piece::NINE_PIN},
  {Piece::WEST_WIND,Piece::WEST_WIND,Piece::WEST_WIND},
  {Piece::EAST_WIND,Piece::EAST_WIND,Piece::EAST_WIND},
  {Piece::SOUTH_WIND,Piece::SOUTH_WIND,Piece::SOUTH_WIND},
  {Piece::NORTH_WIND,Piece::NORTH_WIND,Piece::NORTH_WIND},
  {Piece::RED_DRAGON,Piece::RED_DRAGON,Piece::RED_DRAGON},
  {Piece::WHITE_DRAGON,Piece::WHITE_DRAGON,Piece::WHITE_DRAGON},
  {Piece::GREEN_DRAGON,Piece::GREEN_DRAGON,Piece::GREEN_DRAGON}
};

const std::vector<std::vector<Piece>> PAIRS = {
  {Piece::ONE_BAMBOO,Piece::ONE_BAMBOO},
  {Piece::TWO_BAMBOO,Piece::TWO_BAMBOO},
  {Piece::THREE_BAMBOO,Piece::THREE_BAMBOO},
  {Piece::FOUR_BAMBOO,Piece::FOUR_BAMBOO},
  {Piece::FIVE_BAMBOO,Piece::FIVE_BAMBOO},
  {Piece::SIX_BAMBOO,Piece::SIX_BAMBOO},
  {Piece::SEVEN_BAMBOO,Piece::SEVEN_BAMBOO},
  {Piece::EIGHT_BAMBOO,Piece::EIGHT_BAMBOO},
  {Piece::NINE_BAMBOO,Piece::NINE_BAMBOO},
  {Piece::ONE_PIN,Piece::ONE_PIN},
  {Piece::TWO_PIN,Piece::TWO_PIN},
  {Piece::THREE_PIN,Piece::THREE_PIN},
  {Piece::FOUR_PIN,Piece::FOUR_PIN},
  {Piece::FIVE_PIN,Piece::FIVE_PIN},
  {Piece::SIX_PIN,Piece::SIX_PIN},
  {Piece::SEVEN_PIN,Piece::SEVEN_PIN},
  {Piece::EIGHT_PIN,Piece::EIGHT_PIN},
  {Piece::NINE_PIN,Piece::NINE_PIN},
  {Piece::ONE_CHARACTER,Piece::ONE_CHARACTER},
  {Piece::TWO_CHARACTER,Piece::TWO_CHARACTER},
  {Piece::THREE_CHARACTER,Piece::THREE_CHARACTER},
  {Piece::FOUR_CHARACTER,Piece::FOUR_CHARACTER},
  {Piece::FIVE_CHARACTER,Piece::FIVE_CHARACTER},
  {Piece::SIX_CHARACTER,Piece::SIX_CHARACTER},
  {Piece::SEVEN_CHARACTER,Piece::SEVEN_CHARACTER},
  {Piece::EIGHT_CHARACTER,Piece::EIGHT_CHARACTER},
  {Piece::NINE_CHARACTER,Piece::NINE_CHARACTER},
  {Piece::EAST_WIND,Piece::EAST_WIND},
  {Piece::SOUTH_WIND,Piece::SOUTH_WIND},
  {Piece::WEST_WIND,Piece::WEST_WIND},
  {Piece::NORTH_WIND,Piece::NORTH_WIND},
  {Piece::RED_DRAGON,Piece::RED_DRAGON},
  {Piece::WHITE_DRAGON,Piece::WHITE_DRAGON},
  {Piece::GREEN_DRAGON,Piece::GREEN_DRAGON}
};

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

auto Mahjong::GetPossibleStdFormHand() -> std::vector<Piece>{
  std::vector<Piece> livingWalls;
  std::vector<Piece> deadWall;
  for(int i = 0; i < 4; i++){
      livingWalls.insert(livingWalls.end(),PIECE_SET.begin(),PIECE_SET.end());
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(livingWalls.begin(),livingWalls.end(),g);
  std::move(livingWalls.begin(),livingWalls.begin()+14,std::back_inserter(deadWall));

  uint8_t pieceCount[256] = {};
  for(const auto & piece : livingWalls){
    pieceCount[piece.toUint8_t()]++;
  }

  std::vector<bool> choosePair = {false,false,false,false,true};
  std::shuffle(choosePair.begin(),choosePair.end(),g);
  std::uniform_int_distribution<> pairChance(0,3);
  std::uniform_int_distribution<> tripletSelection(0,54);
  std::uniform_int_distribution<> pairSelection(0,33);

  std::vector<Piece> hand;
  for(bool choice : choosePair){
    if(choice){
      int pair = pairSelection(g);
      while(pieceCount[PAIRS[pair][0].toUint8_t()] < 2){
        pair = pairSelection(g);
      }
      hand.push_back(PAIRS[pair][0]);
      hand.push_back(PAIRS[pair][1]);
      pieceCount[PAIRS[pair][0].toUint8_t()] -= 2;
    }else{
      int triplet = tripletSelection(g);
      while((triplet <= 20 && 
              (pieceCount[TRIPLETS[triplet][0].toUint8_t()] < 1 ||
                pieceCount[TRIPLETS[triplet][1].toUint8_t()] < 1 ||
                pieceCount[TRIPLETS[triplet][2].toUint8_t()] < 1)) || 
              (triplet > 20 && pieceCount[TRIPLETS[triplet][0].toUint8_t()] < 3)){
        triplet = tripletSelection(g);
      }
      if(triplet <= 20){
        pieceCount[TRIPLETS[triplet][0].toUint8_t()]--;
        pieceCount[TRIPLETS[triplet][1].toUint8_t()]--;
        pieceCount[TRIPLETS[triplet][2].toUint8_t()]--;
      }else{
        pieceCount[TRIPLETS[triplet][0].toUint8_t()]-=3;
      }
      hand.push_back(TRIPLETS[triplet][0]);
      hand.push_back(TRIPLETS[triplet][1]);
      hand.push_back(TRIPLETS[triplet][2]);
    }
  }
  return hand;
}
