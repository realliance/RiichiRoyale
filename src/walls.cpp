#include <random>
#include <iterator>
#include <algorithm>
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
    return PieceType::ERROR_PIECE;
}

std::vector<Piece> Walls::TakeHand(){
    if(livingWalls.size() < 13){
        return {};
    }
    std::vector<Piece> hand;
    std::move(livingWalls.begin(), livingWalls.begin() + 13, std::back_inserter(hand));
    return hand;
}

Piece Walls::TakeReplacementTile() {
    if(replacements < 1){
        return PieceType::ERROR_PIECE;
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
