#include <iostream>
#include <map>

#include "stdform.h"
#include "piecetype.h"
#include "hand.h"

auto isInStdForm(const std::vector<Piece>& hand) -> bool{
  std::vector<Piece> pieces;
  uint8_t pieceCount[256] = {};
  for(const auto & piece: hand){
    pieceCount[piece.value()]++;
    if(pieceCount[piece.value()] == 1){
      pieces.push_back(piece);
    }
  }
  std::vector<Meld> handComp;
  Piece pair = ERROR_PIECE;
  std::cout << "============" << std::endl;
  for(const auto & piece: pieces){
    if(!piece.isHonor()){

      while(pieceCount[piece.value()] > 0 && pieceCount[(piece+1).value()] > 0 && pieceCount[(piece+2).value()] > 0 && (pieceCount[piece.value()] != 3 || pieceCount[piece.value()+3] != 1)){
        handComp.push_back(Meld{Chi,{piece,piece+1,piece+2}});
        pieceCount[piece.value()]--;
        pieceCount[(piece+1).value()]--;
        pieceCount[(piece+2).value()]--;
        std::cout << "chi: (" << piece.toStr() << ", " << (piece+1).toStr() << ", " << (piece+2).toStr() << ")" << std::endl;
      }

      if(pieceCount[piece.value()] >= 3 
          // || 
          // (
          //   pieceCount[piece.value()] == 3 
          //   && 
          //   ((
          //       (pieceCount[(piece+1).value()] == 0 || pieceCount[(piece+1).value()] == 3) ||
          //       (pieceCount[(piece+1).value()] > 0 && pieceCount[(piece+2).value()] > 0 && pieceCount[(piece+3).value()] > 0)
          //     ) 
          //     &&
          //     (
          //       (pieceCount[(piece-1).value()] == 0 || pieceCount[(piece+1).value()] == 3) ||
          //       (pieceCount[(piece-1).value()] > 0 && pieceCount[(piece-2).value()] > 0 && pieceCount[(piece-3).value()] > 0)
          //   ))
          // )
        )
      {
        handComp.push_back(Meld{Pon,{piece,piece,piece}});
        pieceCount[piece.value()] -= 3;
        std::cout << "pon: (" << piece.toStr() << ", " << piece.toStr() << ", " << piece.toStr() << ")" << std::endl;
      }

    }else if(pieceCount[piece.value()] == 3 ){
      handComp.push_back(Meld{Pon,{piece,piece,piece}});
      pieceCount[piece.value()] -= 3;
      std::cout << "pon: (" << piece.toStr() << ", " << piece.toStr() << ", " << piece.toStr() << ")" << std::endl;
    }
    if(pieceCount[piece.value()] == 2 && pair == ERROR_PIECE){
      pair = piece;
      pieceCount[piece.value()]-=2;
      std::cout << "pair: (" << piece.toStr() << ", " << piece.toStr() << ")" << std::endl;
    }
    if(pieceCount[piece.value()] == 0){
      continue;
    }
    std::cout << "FAIL: " << piece.toStr() << "[" << int(pieceCount[piece.value()]) << "]" << std::endl;
    std::cout << "NEARBY (-2,-1,1,2): " 
              << int((piece-2).value()) << ":" << (piece-2).toStr() << "[" << int(pieceCount[(piece-2).value()]) << "]" << ", " 
              << int((piece-1).value()) << ":" << (piece-1).toStr()<< "[" << int(pieceCount[(piece-1).value()]) << "]" << ", " 
              << int((piece+1).value()) << ":" << (piece+1).toStr() << "[" << int(pieceCount[(piece+1).value()]) << "]" << ", " 
              << int((piece+2).value()) << ":" << (piece+2).toStr() << "[" << int(pieceCount[(piece+2).value()]) << "]" << ")" << std::endl;
    return false;
  }
  return true;
}
