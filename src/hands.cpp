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
#include "winds.h"


#include "mahjongns.h"
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



std::vector<yakuFunc> yakuFunctions = {
  isFullyConcealedHand, isAllSimples,isWhiteDragonPon,
  isGreenDragonPon,isRedDragonPon,isSeatWind,
  isPrevalentWind,isAfterAKan,isRobbingAKan,isBottomOfTheSea,
  isThirteenOrphans
};

namespace Mahjong {

  auto isComplete(const GameState& state, int player) -> bool{
    const std::vector<const Node*> branch root = breakdownHand(state.hands[state.currentPlayer].live);
    if(isInAValidFormat(state, player, root) != Complete){
      return false;
    }
    for(auto yakuFunction : yakuFunctions){
      if(yakuFunction(state,player, root) == Complete){
        return true;
      }
    }
    return false;
  }

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

  //this is an extrememly inefficient algorithm but it's probably good enough for 
  //the frequency it needs to be ran
  //will revisit if necessary 
  //assumption is 14 piece hand
  auto isInTenpai(std::vector<Piece> hand, bool allWaits = false) -> std::vector<Piece> {
    int8_t counts[256] = {};
    bool removedbefore[256] = {};
    std::vector<Piece> waits;
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
          waits.push_back(p);
          if(!allWaits){
            return waits;
          }
        }
        hand.pop_back();
      }
      hand.push_back(removed);
    }
    return waits;
  }

  auto isInStdForm(const GameState& state, int player, const Node* root) -> HandState{
    HandState hstate = DoesNotSatisfy;
    if(root->IsComplete()){
      hstate = Complete;
    }else if(!isInTenpai(state.hands[player].live).empty()){
      hstate = Tenpai;
    }
    delete root;
    return hstate;
  }

  auto isInAValidFormat(const GameState& state, int player, const Node* root) -> HandState{
    HandState format = isSevenPairs(state, player, {});
    if(format == Tenpai || format == Complete){
      return format;
    }
    format = isThirteenOrphans(state, player, {});
    if(format == Tenpai || format == Complete){
      return format;
    }
    format = isInStdForm(state, player, root);
    if(format == Tenpai || format == Complete){
      return format;
    }
    return DoesNotSatisfy;
  }

  auto isRiichi(const GameState& state, int player, const std::vector<const Node*>) -> HandState{
    if(state.hands[player].open){
      return DoesNotSatisfy;
    }
    if(state.walls.GetRemainingPieces() < 4){
      return DoesNotSatisfy;
    }
    if(!isInTenpai(state.hands[player].live).empty()){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isFullyConcealedHand(const GameState& state, int player, const std::vector<const Node*>) -> HandState{
    if(state.currentPlayer == player && !state.hands[player].open){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isPinfu(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    if(state.hands[player].open){
      return DoesNotSatisfy;
    }
    for(const auto& node : branch){
      if(node->type == Node::PonSet){
        return DoesNotSatisfy;
      }
      if(node->type == Node::Pair){
        if(node->start == Piece::RED_DRAGON || 
           node->start == Piece::WHITE_DRAGON || 
           node->start == Piece::GREEN_DRAGON)
        {
          return DoesNotSatisfy;
        }
        if(node->start == Piece::SOUTH_WIND && state.roundNum > 3){
          return DoesNotSatisfy;
        }
        if(node->start == Piece::EAST_WIND && state.roundNum < 4){
          return DoesNotSatisfy;
        }
      }
    }
    auto waits = isInTenpai(state.hands[player].live,true);
    if(waits.size() == 1){
      return DoesNotSatisfy;
    }
    return Satisfies;
  }

  auto isPureDoubleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    if(state.hands[player].open){
      return DoesNotSatisfy;
    }
    for(int i = 0; i < branch.size(); i++){
      if(branch[i]->type != Node::ChiSet){
        continue;
      }
      for(int j = 0; j < branch.size(); j++){
        if(i==j ){
          continue;
        }
        if(*branch[i] == *branch[j]){
          return Satisfies;
        }
      }
    }
    return DoesNotSatisfy;
  }

  auto isAllSimples(const GameState& state, int player, const std::vector<const Node*>) -> HandState{
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

  auto isMixedTripleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    bool bambooChi[9] = {};
    bool charChi[9] = {};
    bool pinChi[9] = {};
    for(const auto& node : branch){
      if(node->type == Node::ChiSet){
        if(node->start.getSuit() == Piece::BAMBOO_SUIT){
          bambooChi[node->start.getPieceNum()] = true;
        }
        if(node->start.getSuit() == Piece::CHARACTER_SUIT){
          charChi[node->start.getPieceNum()] = true;
        }
        if(node->start.getSuit() == Piece::PIN_SUIT){
          pinChi[node->start.getPieceNum()] = true;
        }
      }
    }
    for(int i = 0; i < 9; i++){
      if(bambooChi[i] && charChi[i] && pinChi[i]){
        return Satisfies;
      }
    }
    return DoesNotSatisfy;
  }
    

  // auto isPureStraight(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  auto isWhiteDragonPon(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    int cnt = countPiece(state, player, Piece::WHITE_DRAGON);
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isGreenDragonPon(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    int cnt = countPiece(state, player, Piece::GREEN_DRAGON);
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isRedDragonPon(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    int cnt = countPiece(state, player, Piece::RED_DRAGON);
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isSeatWind(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    int cnt = countPiece(state, player, Piece::fromWind(GetSeat(state.roundNum,player)));
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isPrevalentWind(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    int cnt = countPiece(state, player, Piece::fromWind(state.roundNum > 3 ? South : East));
    if(cnt > 2){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  // auto isOutsideHand(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
  // }

  auto isAfterAKan(const GameState& state, int) -> HandState{
    if(state.nextState == KanDiscard){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isRobbingAKan(const GameState& state, int) -> HandState{
    if(state.nextState == KanDiscard){
      return Satisfies;
    }
    // if(state.currentState == AfterConcealedKanDiscard && isThirteenOrphans(state, player)){
    //   return Satisfies;
    // }
    return DoesNotSatisfy;
  }

  auto isBottomOfTheSea(const GameState& state, int) -> HandState{
    if(state.walls.GetRemainingPieces() == 0){
      return Satisfies;
    }
    return DoesNotSatisfy;
  }

  auto isSevenPairs(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
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

  // auto isTriplePon(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isThreeConcealedPons(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isAllPons(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  auto isHalfFlush(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
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

  // auto isLittleThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isAllTerminalsAndHonors(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isTerminalsInAllSets(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isTwicePureDoubleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isBlessingOfMan(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isFullFlush(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  auto isThirteenOrphans(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
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

  // auto isNineGates(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isBlessingOfHeaven(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isBlessingOfEarth(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isFourConcealedPon(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isFourKans(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isAllGreen(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isAllTerminals(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isAllHonors(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isBigThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isLittleFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

  // auto isBigFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> HandState{
    
  // }

};
