#include <random>
#include <map>
#include <algorithm>
#include <string>
#include "walls.h"

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

Walls::Walls(){
    for(int i = 0; i < 4; i++){
        livingWalls.insert(livingWalls.end(),PIECE_SET.begin(),PIECE_SET.end());
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(livingWalls.begin(), livingWalls.end(), g);
    std::move(livingWalls.begin(), livingWalls.begin()+14, std::back_inserter(deadWall));
}

Piece Walls::TakePiece(){
    if(livingWalls.size() > 0){
        Piece p = livingWalls.front();
        livingWalls.pop_front();
        return p;
    }
    return Pieces::ERROR_PIECE;
}

std::vector<Piece> Walls::TakeHand(){
    if(livingWalls.size() < 14){
        return {};
    }
    std::vector<Piece> hand;
    std::move(livingWalls.begin(), livingWalls.begin()+14, std::back_inserter(hand));
    return hand;
}

Piece Walls::TakeReplacementTile() {
    if(replacements < 1){
        return Pieces::ERROR_PIECE;
    }
    replacements--;
    Piece p = deadWall.front();
    deadWall.pop_front();
    deadWall.push_back(livingWalls.back());
    livingWalls.pop_back();
    return p;
}

std::vector<Piece> Walls::GetDoras() const{
    std::vector<Piece> doras;
    std::copy_n(deadWall.begin()+replacements, doraCount, std::back_inserter(doras));
    return doras;
}

std::vector<Piece> Walls::GetUraDoras() const{
    std::vector<Piece> doras;
    std::copy_n(deadWall.begin()+replacements+doraCount, doraCount, std::back_inserter(doras));
    return doras;
}

int Walls::GetRemainingPieces() const{
    return livingWalls.size();
}

const std::map<Piece,std::string> FILE_MAP = {
    {RED_DRAGON , "Chun.svg"},
    {WHITE_DRAGON , "Haku.svg"},
    {GREEN_DRAGON , "Hatsu.svg"},
    {ONE_CHARACTER , "Man1.svg"},
    {TWO_CHARACTER , "Man2.svg"},
    {THREE_CHARACTER , "Man3.svg"},
    {FOUR_CHARACTER , "Man4.svg"},
    {RED_FIVE_CHARACTER , "Man5-Dora.svg"},
    {FIVE_CHARACTER , "Man5.svg"},
    {SIX_CHARACTER , "Man6.svg"},
    {SEVEN_CHARACTER , "Man7.svg"},
    {EIGHT_CHARACTER , "Man8.svg"},
    {NINE_CHARACTER , "Man9.svg"},
    {SOUTH_WIND , "Nan.svg"},
    {NORTH_WIND , "Pei.svg"},
    {ONE_PIN , "Pin1.svg"},
    {TWO_PIN , "Pin2.svg"},
    {THREE_PIN , "Pin3.svg"},
    {FOUR_PIN , "Pin4.svg"},
    {RED_FIVE_PIN , "Pin5-Dora.svg"},
    {FIVE_PIN , "Pin5.svg"},
    {SIX_PIN , "Pin6.svg"},
    {SEVEN_PIN , "Pin7.svg"},
    {EIGHT_PIN , "Pin8.svg"},
    {NINE_PIN , "Pin9.svg"},
    {WEST_WIND , "Shaa.svg"},
    {ONE_BAMBOO , "Sou1.svg"},
    {TWO_BAMBOO , "Sou2.svg"},
    {THREE_BAMBOO , "Sou3.svg"},
    {FOUR_BAMBOO , "Sou4.svg"},
    {RED_FIVE_BAMBOO , "Sou5-Dora.svg"},
    {FIVE_BAMBOO , "Sou5.svg"},
    {SIX_BAMBOO , "Sou6.svg"},
    {SEVEN_BAMBOO , "Sou7.svg"},
    {EIGHT_BAMBOO , "Sou8.svg"},
    {NINE_BAMBOO , "Sou9.svg"},
    {EAST_WIND , "Ton.svg"},
};

std::string Walls::path;

std::string Walls::GetPath(Piece piece){
 return path + "/" + FILE_MAP.at(piece);
}

void Walls::SetPath(std::string filePath){
    if(filePath.back() == '/'){
        filePath.pop_back();
    }
    path = filePath;
}

void Walls::Sort(std::vector<Piece>& pieces){
  // sort using a custom function object
  struct {
      bool operator()(Piece a, Piece b) const{   
        if(getSuit(a) != getSuit(b)){
          return getSuit(a) > getSuit(b);
        }
        return getPieceNum(a) < getPieceNum(b);
      }   
  } pieceSort;
  std::sort(pieces.begin(), pieces.end(), pieceSort);
}
