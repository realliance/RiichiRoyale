#include "hands.h"
#include <stddef.h>     // for size_t
#include <array>        // for array, array<>::value_type
#include <map>          // for map
#include <vector>       // for vector
#include "gamestate.h"  // for GameState, AfterConcealedKanDiscard, AfterKan...
#include "hand.h"       // for Hand
#include "handnode.h"   // for Node, Single
#include "handtree.h"   // for breakdownHand
#include "player.h"     // for Player
#include "walls.h"      // for Walls

auto countPiece(const GameState& state, int player, Piece p) -> int{
  int count = 0;
  for(const auto & piece : state.hands[player]){
    if(piece == p){
      count++;
    }
  }
  return count;
}

using yakuFunc = HandState(*)(const GameState&, int);

std::vector<yakuFunc> yakuFunctions = {
  isFullyConcealedHand, isAllSimples,isWhiteDragonPon,
  isGreenDragonPon,isRedDragonPon,isSeatWind,
  isPrevalentWind,isAfterAKan,isRobbingAKan,isBottomOfTheSea,
  isThirteenOrphans
};

auto isComplete(const GameState& state, int player) -> bool{
  if(isInAValidFormat(state, player) != Complete){
    return false;
  }
  for(auto yakuFunction : yakuFunctions){
    if(yakuFunction(state,player) == Complete){
      return true;
    }
  }
  return false;
}

auto isInStdForm(const GameState& state, int player) -> HandState{
  Node* root = breakdownHand(state.hands[player].live);
  if(root->leaves[0]->type != Single){
    delete root;
    return Complete;
  }
  if(root->leaves[0]->leaves.size() > 1 && root->leaves[0]->leaves[0]->type != Single){
    delete root;
    return Tenpai;
  }
  delete root;
  return DoesNotSatisfy;
}

auto isInAValidFormat(const GameState& state, int player) -> HandState{
  HandState format = isSevenPairs(state, player);
  if(format == Tenpai || format == Complete){
    return format;
  }
  format = isThirteenOrphans(state, player);
  if(format == Tenpai || format == Complete){
    return format;
  }
  format = isInStdForm(state, player);
  if(format == Tenpai || format == Complete){
    return format;
  }
  return DoesNotSatisfy;
}

// auto isRiichi(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
//   if(state.walls.GetRemainingPieces() < 4){
//     return DoesNotSatisfy;
//   }
//   return Satisfies;
// }

auto isFullyConcealedHand(const GameState& state, int player) -> HandState{
  if(state.currentPlayer == player && !state.hands[player].open){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

// auto isPinfu(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

// auto isPureDoubleChi(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

auto isAllSimples(const GameState& state, int player) -> HandState{
  for(const auto & piece : state.hands[player]){
    if(piece.isHonor() || piece.isTerminal()){
      return DoesNotSatisfy;
    }
  }
  return Satisfies;
}

// auto isMixedTripleChi(const GameState& state, int player) -> HandState{
// }
  

//// auto isPureStraight(const GameState& state, int player) -> HandState{
//// }

auto isWhiteDragonPon(const GameState& state, int player) -> HandState{
  int cnt = countPiece(state, player, WHITE_DRAGON);
  if(cnt > 2){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isGreenDragonPon(const GameState& state, int player) -> HandState{
  int cnt = countPiece(state, player, GREEN_DRAGON);
  if(cnt > 2){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isRedDragonPon(const GameState& state, int player) -> HandState{
  int cnt = countPiece(state, player, RED_DRAGON);
  if(cnt > 2){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isSeatWind(const GameState& state, int player) -> HandState{
  int cnt = countPiece(state, player, Piece::fromWind(state.players[player].seat));
  if(cnt > 2){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isPrevalentWind(const GameState& state, int player) -> HandState{
  int cnt = countPiece(state, player, Piece::fromWind(state.prevalentWind));
  if(cnt > 2){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

// auto isOutsideHand(const GameState& state, int player) -> HandState{
// }

auto isAfterAKan(const GameState& state, int player) -> HandState{
  if(state.currentState == AfterReplacementDraw){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isRobbingAKan(const GameState& state, int player) -> HandState{
  if(state.currentState == AfterKanDiscard){
    return Satisfies;
  }
  if(state.currentState == AfterConcealedKanDiscard && isThirteenOrphans(state, player)){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isBottomOfTheSea(const GameState& state, int player) -> HandState{
  if(state.walls.GetRemainingPieces() == 0){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

auto isSevenPairs(const GameState& state, int player) -> HandState{
  if(state.hands[player].open){
    return DoesNotSatisfy;
  }
  for(size_t i = 0; i < state.hands[player].live.size(); i += 2){
    if(state.hands[player].live[i] != state.hands[player].live[i+1]){
      return DoesNotSatisfy;
    }
  }
  return Satisfies;
}

// auto isTriplePon(const GameState& state, int player) -> HandState{
  
// }

// auto isThreeConcealedPons(const GameState& state, int player) -> HandState{
  
// }

// auto isAllPons(const GameState& state, int player) -> HandState{
  
// }

auto isHalfFlush(const GameState& state, int player) -> HandState{
  int suit;
  bool suitSet = false;
  bool honors = false;
  for(const auto & piece : state.hands[player]){
    if(piece.isHonor()){
      honors = true;
      continue;
    }
    if(!suitSet){
      suit = piece.getSuit();
      continue;
    }
    if(suit != piece.getSuit()){
      return DoesNotSatisfy;
    }
  }
  if(honors){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

// auto isLittleThreeDragons(const GameState& state, int player) -> HandState{
  
// }

// auto isAllTerminalsAndHonors(const GameState& state, int player) -> HandState{
  
// }

// auto isTerminalsInAllSets(const GameState& state, int player) -> HandState{
  
// }

// auto isTwicePureDoubleChi(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

// auto isBlessingOfMan(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

// auto isFullFlush(const GameState& state, int player) -> HandState{
  
// }

auto isThirteenOrphans(const GameState& state, int player) -> HandState{
  if(state.hands[player].open){
    return DoesNotSatisfy;
  }
  std::map<Piece,bool> pieces = {
    {ONE_CHARACTER, false}, {NINE_CHARACTER, false}, {ONE_PIN, false},
    {NINE_PIN, false}, {ONE_BAMBOO, false}, {NINE_BAMBOO, false},
    {EAST_WIND, false}, {SOUTH_WIND, false},{WEST_WIND, false},
    {NORTH_WIND, false}, {RED_DRAGON, false}, {WHITE_DRAGON, false},
    {GREEN_DRAGON, false}
  };
  bool duplicate = false;
  for(const auto & piece : state.hands[player].live){
    if(pieces.contains(piece)){
      if(pieces[piece]){
        duplicate = true;
      }else{
        pieces[piece] = true;
      }
    }
  }
  int misses = 0;
  for(const auto& [piece,match] : pieces){
    if(!match){
      misses++;
    }
  }
  if(misses == 0 && duplicate){
    return Satisfies;
  }
  return DoesNotSatisfy;
}

// auto isNineGates(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

// auto isBlessingOfHeaven(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

// auto isBlessingOfEarth(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

// auto isFourConcealedPon(const GameState& state, int player) -> HandState{
//   if(state.hands[player].open){
//     return DoesNotSatisfy;
//   }
// }

// auto isFourKans(const GameState& state, int player) -> HandState{
  
// }

// auto isAllGreen(const GameState& state, int player) -> HandState{
  
// }

// auto isAllTerminals(const GameState& state, int player) -> HandState{
  
// }

// auto isAllHonors(const GameState& state, int player) -> HandState{
  
// }

// auto isBigThreeDragons(const GameState& state, int player) -> HandState{
  
// }

// auto isLittleFourWinds(const GameState& state, int player) -> HandState{
  
// }

// auto isBigFourWinds(const GameState& state, int player) -> HandState{
  
// }
