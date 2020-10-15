#pragma once
#include <algorithm>
#include <stdint.h>

#include "hands.h"
#include "gamestate.h"

auto hasAtLeast(const RoundState* state, Piece p, int num) -> bool{
  int count = 0;
  for(const auto & piece : state->currentPlayer->hand){
    if(piece == p){
      count++;
      if(count == num){
        return true;
      }
    }
  }
}

auto isInStdForm(const RoundState* state) -> HandState{
  std::vector<Piece> pieces;
  uint8_t pieceCount[256] = {};
  for(const auto & piece: state->currentPlayer->hand){
    pieceCount[piece.value()]++;
    if(pieceCount[piece.value()] == 1){
      pieces.push_back(piece);
    }
  }
  std::vector<Meld> handComp;
  Piece pair = ERROR_PIECE;
  for(const auto & piece: pieces){
    if(pieceCount[piece.value()] == 3){
      handComp.push_back(Meld{Pon,{piece,piece,piece}});
      pieceCount[piece.value()] = 0;
      continue;
    }
    if(pieceCount[piece.value()] > 0 && pieceCount[piece.value()+1] > 0 && pieceCount[piece.value()+2] > 0){
      handComp.push_back(Meld{Chi,{piece,piece+1,piece+2}});
      pieceCount[piece.value()]--;
      pieceCount[piece.value()+1]--;
      pieceCount[piece.value()+2]--;
      continue;
    }
    if(pieceCount[piece.value()-1] > 0 && pieceCount[piece.value()] > 0 && pieceCount[piece.value()+1] > 0){
      handComp.push_back(Meld{Chi,{piece,piece+1,piece+2}});
      pieceCount[piece.value()]--;
      pieceCount[piece.value()+1]--;
      pieceCount[piece.value()+2]--;
      continue;
    }
    if(pieceCount[piece.value()-2] > 0 && pieceCount[piece.value()-1] > 0 && pieceCount[piece.value()] > 0){
      handComp.push_back(Meld{Chi,{piece,piece+1,piece+2}});
      pieceCount[piece.value()]--;
      pieceCount[piece.value()+1]--;
      pieceCount[piece.value()+2]--;
      continue;
    }
    if(pieceCount[piece.value()] == 2 && pair == ERROR_PIECE){
      pair = piece;
      pieceCount[piece.value()]-=2;
      continue;
    }
    return DoesNotSatisfy;
  }
  return Satisfies;
}

auto isInAValidFormat(const RoundState* state) -> HandState{
  HandState format = isSevenPairs(state);
  if(format == Tenpai || format == Complete){
    return format;
  }
  format = isThirteenOrphans(state);
  if(format == Tenpai || format == Complete){
    return format;
  }
  format = isInStdForm(state);
  if(format == Tenpai || format == Complete){
    return format;
  }
  return DoesNotSatisfy;
}

auto isRiichi(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
  if(state->walls.GetRemainingPieces() < 4){
    return DoesNotSatisfy;
  }
  return Satisfies;
}

auto isFullyConcealedHand(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
  if(state->lastCall != state->turnCount){
    return DoesNotSatisfy;
  }
  return Complete;
}

auto isPinfu(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isPureDoubleChi(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isAllSimples(const RoundState* state) -> HandState{
  for(const auto & piece : state->currentPlayer->hand){
    if(piece.isHonor() || piece.isTerminal()){
      return DoesNotSatisfy;
    }
  }
  return Satisfies;
}

auto isMixedTripleChi(const RoundState* state) -> HandState{
  const Hand* hand = &state->currentPlayer->hand;
  int pieces[10] = {};
  int count;
  for(const auto & piece : state->currentPlayer->hand.live){
    if(!piece.isHonor()){
      count++;
      pieces[piece.value()]++;
    }
  }
  int chiMatches[8] = {};
  for(const auto & meld : state->currentPlayer->hand.melds){
    if(meld.type == Chi){
      if(++chiMatches[std::min_element(meld.pieces.begin(),meld.pieces.end())->value()] == 3){
        return Satisfies;
      }
    }
  }

  for(int i = 0; i < 8; i++){
    if(chiMatches[i] >= 2 && pieces[i] >= 1 && pieces[i+1] >= 1 && pieces[i+2] >= 1){
        return Satisfies;
    }
    if(chiMatches[i] >= 1 && pieces[i] >= 2 && pieces[i+1] >= 2 && pieces[i+2] >= 2){
        return Satisfies;
    }
    if(pieces[i] >= 3 && pieces[i+1] >= 3 && pieces[i+2] >= 3){
        return Satisfies;
    }
  }
  return DoesNotSatisfy;
}
  

auto isPureStraight(const RoundState* state) -> HandState{
  
}

auto isWhiteDragonPon(const RoundState* state) -> HandState{
  return hasAtLeast(state, WHITE_DRAGON, 3) ? Satisfies : DoesNotSatisfy;
}

auto isGreenDragonPon(const RoundState* state) -> HandState{
  return hasAtLeast(state, GREEN_DRAGON, 3) ? Satisfies : DoesNotSatisfy;
}

auto isRedDragonPon(const RoundState* state) -> HandState{
  return hasAtLeast(state, RED_DRAGON, 3) ? Satisfies : DoesNotSatisfy;
}

auto isSeatWind(const RoundState* state) -> HandState{
  return hasAtLeast(state, Piece::fromWind(state->currentPlayer->seat), 3) ? Satisfies : DoesNotSatisfy;
}

auto isPrevalentWind(const RoundState* state) -> HandState{
  return hasAtLeast(state, Piece::fromWind(state->prevalentWind), 3) ? Satisfies : DoesNotSatisfy;
}

auto isOutsideHand(const RoundState* state) -> HandState{
  
}

auto isAfterAKan(const RoundState* state) -> HandState{
  if(state->currentState == AfterKanPlayerTurn){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isRobbingAKan(const RoundState* state) -> HandState{
  if(state->currentState != KanDiscard){
    return DoesNotSatisfy;
  }
  if(isInAValidFormat(state))
}

auto isBottomOfTheSea(const RoundState* state) -> HandState{
  
}

auto isSevenPairs(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isTriplePon(const RoundState* state) -> HandState{
  
}

auto isThreeConcealedPons(const RoundState* state) -> HandState{
  
}

auto isAllPons(const RoundState* state) -> HandState{
  
}

auto isHalfFlush(const RoundState* state) -> HandState{
  
}

auto isLittleThreeDragons(const RoundState* state) -> HandState{
  
}

auto isAllTerminalsAndHonors(const RoundState* state) -> HandState{
  
}

auto isTerminalsInAllSets(const RoundState* state) -> HandState{
  
}

auto isTwicePureDoubleChi(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isBlessingOfMan(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isFullFlush(const RoundState* state) -> HandState{
  
}

auto isThirteenOrphans(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isNineGates(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isBlessingOfHeaven(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isBlessingOfEarth(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isFourConcealedPon(const RoundState* state) -> HandState{
  if(state->currentPlayer->hand.open){
    return DoesNotSatisfy;
  }
}

auto isFourKans(const RoundState* state) -> HandState{
  
}

auto isAllGreen(const RoundState* state) -> HandState{
  
}

auto isAllTerminals(const RoundState* state) -> HandState{
  
}

auto isAllHonors(const RoundState* state) -> HandState{
  
}

auto isBigThreeDragons(const RoundState* state) -> HandState{
  
}

auto isLittleFourWinds(const RoundState* state) -> HandState{
  
}

auto isBigFourWinds(const RoundState* state) -> HandState{
  
}
