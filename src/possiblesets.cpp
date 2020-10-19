#include "possiblesets.h"

#include <stdint.h>            // for uint8_t
#include <algorithm>           // for shuffle, move
#include <iterator>            // for back_insert_iterator, back_inserter
#include <memory>              // for allocator_traits<>::value_type
#include <random>              // for mt19937, uniform_int_distribution, ran...

#include "piecetype.h"         // for Piece, FIVE_BAMBOO, FIVE_CHARACTER

const std::vector<std::vector<Piece>> TRIPLETS = {
  {ONE_BAMBOO,TWO_BAMBOO,THREE_BAMBOO},
  {TWO_BAMBOO,THREE_BAMBOO,FOUR_BAMBOO},
  {THREE_BAMBOO,FOUR_BAMBOO,FIVE_BAMBOO},
  {FOUR_BAMBOO,FIVE_BAMBOO,SIX_BAMBOO},
  {FIVE_BAMBOO,SIX_BAMBOO,SEVEN_BAMBOO},
  {SIX_BAMBOO,SEVEN_BAMBOO,EIGHT_BAMBOO},
  {SEVEN_BAMBOO,EIGHT_BAMBOO,NINE_BAMBOO},
  {ONE_CHARACTER,TWO_CHARACTER,THREE_CHARACTER},
  {TWO_CHARACTER,THREE_CHARACTER,FOUR_CHARACTER},
  {THREE_CHARACTER,FOUR_CHARACTER,FIVE_CHARACTER},
  {FOUR_CHARACTER,FIVE_CHARACTER,SIX_CHARACTER},
  {FIVE_CHARACTER,SIX_CHARACTER,SEVEN_CHARACTER},
  {SIX_CHARACTER,SEVEN_CHARACTER,EIGHT_CHARACTER},
  {SEVEN_CHARACTER,EIGHT_CHARACTER,NINE_CHARACTER},
  {ONE_PIN,TWO_PIN,THREE_PIN},
  {TWO_PIN,THREE_PIN,FOUR_PIN},
  {THREE_PIN,FOUR_PIN,FIVE_PIN},
  {FOUR_PIN,FIVE_PIN,SIX_PIN},
  {FIVE_PIN,SIX_PIN,SEVEN_PIN},
  {SIX_PIN,SEVEN_PIN,EIGHT_PIN},
  {SEVEN_PIN,EIGHT_PIN,NINE_PIN},
  {ONE_BAMBOO,ONE_BAMBOO,ONE_BAMBOO},
  {TWO_BAMBOO,TWO_BAMBOO,TWO_BAMBOO},
  {THREE_BAMBOO,THREE_BAMBOO,THREE_BAMBOO},
  {FOUR_BAMBOO,FOUR_BAMBOO,FOUR_BAMBOO},
  {FIVE_BAMBOO,FIVE_BAMBOO,FIVE_BAMBOO},
  {SIX_BAMBOO,SIX_BAMBOO,SIX_BAMBOO},
  {SEVEN_BAMBOO,SEVEN_BAMBOO,SEVEN_BAMBOO},
  {EIGHT_BAMBOO,EIGHT_BAMBOO,EIGHT_BAMBOO},
  {NINE_BAMBOO,NINE_BAMBOO,NINE_BAMBOO},
  {ONE_CHARACTER,ONE_CHARACTER,ONE_CHARACTER},
  {TWO_CHARACTER,TWO_CHARACTER,TWO_CHARACTER},
  {THREE_CHARACTER,THREE_CHARACTER,THREE_CHARACTER},
  {FOUR_CHARACTER,FOUR_CHARACTER,FOUR_CHARACTER},
  {FIVE_CHARACTER,FIVE_CHARACTER,FIVE_CHARACTER},
  {SIX_CHARACTER,SIX_CHARACTER,SIX_CHARACTER},
  {SEVEN_CHARACTER,SEVEN_CHARACTER,SEVEN_CHARACTER},
  {EIGHT_CHARACTER,EIGHT_CHARACTER,EIGHT_CHARACTER},
  {NINE_CHARACTER,NINE_CHARACTER,NINE_CHARACTER},
  {ONE_PIN,ONE_PIN,ONE_PIN},
  {TWO_PIN,TWO_PIN,TWO_PIN},
  {THREE_PIN,THREE_PIN,THREE_PIN},
  {FOUR_PIN,FOUR_PIN,FOUR_PIN},
  {FIVE_PIN,FIVE_PIN,FIVE_PIN},
  {SIX_PIN,SIX_PIN,SIX_PIN},
  {SEVEN_PIN,SEVEN_PIN,SEVEN_PIN},
  {EIGHT_PIN,EIGHT_PIN,EIGHT_PIN},
  {NINE_PIN,NINE_PIN,NINE_PIN},
  {WEST_WIND,WEST_WIND,WEST_WIND},
  {EAST_WIND,EAST_WIND,EAST_WIND},
  {SOUTH_WIND,SOUTH_WIND,SOUTH_WIND},
  {NORTH_WIND,NORTH_WIND,NORTH_WIND},
  {RED_DRAGON,RED_DRAGON,RED_DRAGON},
  {WHITE_DRAGON,WHITE_DRAGON,WHITE_DRAGON},
  {GREEN_DRAGON,GREEN_DRAGON,GREEN_DRAGON}
};

const std::vector<std::vector<Piece>> PAIRS = {
  {ONE_BAMBOO,ONE_BAMBOO},
  {TWO_BAMBOO,TWO_BAMBOO},
  {THREE_BAMBOO,THREE_BAMBOO},
  {FOUR_BAMBOO,FOUR_BAMBOO},
  {FIVE_BAMBOO,FIVE_BAMBOO},
  {SIX_BAMBOO,SIX_BAMBOO},
  {SEVEN_BAMBOO,SEVEN_BAMBOO},
  {EIGHT_BAMBOO,EIGHT_BAMBOO},
  {NINE_BAMBOO,NINE_BAMBOO},
  {ONE_PIN,ONE_PIN},
  {TWO_PIN,TWO_PIN},
  {THREE_PIN,THREE_PIN},
  {FOUR_PIN,FOUR_PIN},
  {FIVE_PIN,FIVE_PIN},
  {SIX_PIN,SIX_PIN},
  {SEVEN_PIN,SEVEN_PIN},
  {EIGHT_PIN,EIGHT_PIN},
  {NINE_PIN,NINE_PIN},
  {ONE_CHARACTER,ONE_CHARACTER},
  {TWO_CHARACTER,TWO_CHARACTER},
  {THREE_CHARACTER,THREE_CHARACTER},
  {FOUR_CHARACTER,FOUR_CHARACTER},
  {FIVE_CHARACTER,FIVE_CHARACTER},
  {SIX_CHARACTER,SIX_CHARACTER},
  {SEVEN_CHARACTER,SEVEN_CHARACTER},
  {EIGHT_CHARACTER,EIGHT_CHARACTER},
  {NINE_CHARACTER,NINE_CHARACTER},
  {EAST_WIND,EAST_WIND},
  {SOUTH_WIND,SOUTH_WIND},
  {WEST_WIND,WEST_WIND},
  {NORTH_WIND,NORTH_WIND},
  {RED_DRAGON,RED_DRAGON},
  {WHITE_DRAGON,WHITE_DRAGON},
  {GREEN_DRAGON,GREEN_DRAGON}
};

const std::vector<Piece> PIECE_SET = {
    ONE_BAMBOO, TWO_BAMBOO, THREE_BAMBOO, FOUR_BAMBOO,
    FIVE_BAMBOO, SIX_BAMBOO, SEVEN_BAMBOO, EIGHT_BAMBOO,
    NINE_BAMBOO, ONE_PIN, TWO_PIN, THREE_PIN, FOUR_PIN,
    FIVE_PIN, SIX_PIN, SEVEN_PIN, EIGHT_PIN, NINE_PIN,
    ONE_CHARACTER, NINE_CHARACTER, TWO_CHARACTER, 
    THREE_CHARACTER, FOUR_CHARACTER, FIVE_CHARACTER,
    SIX_CHARACTER, SEVEN_CHARACTER, EIGHT_CHARACTER, 
    WHITE_DRAGON, GREEN_DRAGON, RED_DRAGON, EAST_WIND,
    SOUTH_WIND, NORTH_WIND, WEST_WIND
};

auto GetPossibleStdFormHand() -> std::vector<Piece>{
  std::vector<Piece> livingWalls;
  std::vector<Piece> deadWall;
  for(int i = 0; i < 4; i++){
      livingWalls.insert(livingWalls.end(),PIECE_SET.begin(),PIECE_SET.end());
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(livingWalls.begin(), livingWalls.end(), g);
  std::move(livingWalls.begin(), livingWalls.begin()+14, std::back_inserter(deadWall));

  uint8_t pieceCount[256] = {};
  for(const auto & piece : livingWalls){
    pieceCount[piece.toUint8_t()]++;
  }

  std::vector<bool> choosePair = {false,false,false,false,true};
  std::shuffle(choosePair.begin(), choosePair.end(), g);
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
