#include "hands.h"
#include <stddef.h>
#include <array>
#include <map>
#include <vector>
#include <ext/alloc_traits.h>
#include "gamestate.h"
#include "hand.h"
#include "handnode.h"
#include "analysis.h"
#include "walls.h"
#include "stateutilities.h"
#include "meld.h"
#include "statefunctions.h"


using namespace Mahjong;

auto Mahjong::countPiece(const GameState& state, int player, Piece p) -> int{
  int count = 0;
  for(const auto & piece : state.hands[player].live){
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

namespace Mahjong {

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
    if(root->leaves[0]->type != Node::Single){
      delete root;
      return Complete;
    }
    if(root->leaves[0]->leaves.size() > 1 && root->leaves[0]->leaves[0]->type != Node::Single){
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
    for(const auto & piece : state.hands[player].live){
      if(piece.isHonor() || piece.isTerminal()){
        return DoesNotSatisfy;
      }
    }
    for(const auto & meld : state.hands[player].melds){
      if(meld.start.isHonor() || meld.start.isTerminal()){
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
    int cnt = countPiece(state, player, Piece::WHITE_DRAGON);
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isGreenDragonPon(const GameState& state, int player) -> HandState{
    int cnt = countPiece(state, player, Piece::GREEN_DRAGON);
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isRedDragonPon(const GameState& state, int player) -> HandState{
    int cnt = countPiece(state, player, Piece::RED_DRAGON);
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isSeatWind(const GameState& state, int player) -> HandState{
    int cnt = countPiece(state, player, Piece::fromWind(GetSeat(state.roundNum,player)));
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isPrevalentWind(const GameState& state, int player) -> HandState{
    int cnt = countPiece(state, player, Piece::fromWind(state.roundNum > 3 ? South : East));
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  // auto isOutsideHand(const GameState& state, int player) -> HandState{
  // }

  auto isAfterAKan(const GameState& state, int player) -> HandState{
    if(state.nextState == KanDiscard){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isRobbingAKan(const GameState& state, int player) -> HandState{
    if(state.nextState == KanDiscard){
      return Satisfies;
    }
    // if(state.currentState == AfterConcealedKanDiscard && isThirteenOrphans(state, player)){
    //   return Satisfies;
    // }
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
    for(const auto & piece : state.hands[player].live){
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
    for(const auto & meld : state.hands[player].melds){
      if(meld.start.isHonor()){
        honors = true;
        continue;
      }
      if(!suitSet){
        suit = meld.start.getSuit();
        continue;
      }
      if(suit != meld.start.getSuit()){
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
      {Piece::ONE_CHARACTER, false}, {Piece::NINE_CHARACTER, false}, {Piece::ONE_PIN, false},
      {Piece::NINE_PIN, false}, {Piece::ONE_BAMBOO, false}, {Piece::NINE_BAMBOO, false},
      {Piece::EAST_WIND, false}, {Piece::SOUTH_WIND, false},{Piece::WEST_WIND, false},
      {Piece::NORTH_WIND, false}, {Piece::RED_DRAGON, false}, {Piece::WHITE_DRAGON, false},
      {Piece::GREEN_DRAGON, false}
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

};
