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
  isPrevalentWind,isAfterAKan,isRobbingAKan,isBottomOfTheSea
};

std::vector<yakuFunc> yakumanFunctions = {
  isThirteenOrphans
};

namespace Mahjong {

  auto scoreHand(const GameState& state, int player) -> score{
    const Node* root = breakdownHand(state.hands[state.currentPlayer].live);
    score s;
    s.han = 0;
    s.yakuman = 0;
    for(const auto& branch : root->AsBranchVectors()){
      int branchhan = 0;
      int branchyakumans = 0;
      if(!root->IsComplete() && 
         !isThirteenOrphans(state,player,branch) &&
         !isSevenPairs(state,player,branch)){
        continue;
      }
      for(auto yakuFunction : yakuFunctions){
        branchhan += yakuFunction(state,player, branch);
      }
      for(auto yakuFunction : yakumanFunctions){
        branchyakumans += yakuFunction(state,player, branch);
      }
      if(s.yakuman == 0){
        s.han = branchhan > s.han ? branchhan : s.han;
        if(branchyakumans > 0){
          s.yakuman = branchyakumans;
          s.han = 0;
        }
      }else{
        s.yakuman = branchyakumans > s.yakuman ? branchyakumans : s.yakuman;
      }
    }
    delete root;
    return s;
  }

  auto isComplete(const GameState& state, int player) -> bool{
    const Node* root = breakdownHand(state.hands[state.currentPlayer].live);
    for(const auto& branch : root->AsBranchVectors()){
      if(!root->IsComplete() && 
         !isThirteenOrphans(state,player,branch) &&
         !isSevenPairs(state,player,branch)){
        continue;
      }
      for(auto yakuFunction : yakuFunctions){
        if(yakuFunction(state,player, branch) > 0){
          delete root;
          return true;
        }
      }
      for(auto yakuFunction : yakumanFunctions){
        if(yakuFunction(state,player, branch) > 0){
          delete root;
          return true;
        }
      }
    }
    delete root;
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

  auto isRiichi(const GameState& state, int player, const std::vector<const Node*>) -> int{
    if(state.hands[player].riichi){
      int han = 1;
      if(state.turnNum < 4 && state.lastCall < 0){
        han++;
      }
      if(state.turnNum - state.hands[player].riichiRound < 5 && state.lastCall < state.hands[player].riichiRound){
        han++;
      }
    }
    return 0;
  }

  auto isFullyConcealedHand(const GameState& state, int player, const std::vector<const Node*>) -> int{
    if(state.currentPlayer == player && !state.hands[player].open){
      return 1;
    }
    return 0;
  }

  auto isPinfu(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(state.hands[player].open){
      return 0;
    }
    for(const auto& node : branch){
      if(node->type == Node::PonSet){
        return 0;
      }
      if(node->type == Node::Pair){
        if(node->start == Piece::RED_DRAGON || 
           node->start == Piece::WHITE_DRAGON || 
           node->start == Piece::GREEN_DRAGON)
        {
          return 0;
        }
        if(node->start == Piece::SOUTH_WIND && state.roundNum > 3){
          return 0;
        }
        if(node->start == Piece::EAST_WIND && state.roundNum < 4){
          return 0;
        }
      }
    }
    auto waits = isInTenpai(state.hands[player].live,true);
    if(waits.size() == 1){
      return 0;
    }
    return 1;
  }

  auto isPureDoubleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(state.hands[player].open){
      return 0;
    }
    for(size_t i = 0; i < branch.size(); i++){
      if(branch[i]->type != Node::ChiSet){
        continue;
      }
      for(size_t j = 0; j < branch.size(); j++){
        if(i==j ){
          continue;
        }
        if(*branch[i] == *branch[j]){
          return 1;
        }
      }
    }
    return 0;
  }

  auto isAllSimples(const GameState& state, int player, const std::vector<const Node*>) -> int{
    for(const auto & piece : state.hands[player].live){
      if(piece.isHonor() || piece.isTerminal()){
        return 0;
      }
    }
    for(const auto & meld : state.hands[player].melds){
      if(meld.start.isHonor() || meld.start.isTerminal()){
        return 0;
      }
    }
    return 1;
  }

  auto isMixedTripleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
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
    for(const auto& meld : state.hands[player].melds){
      if(meld.type == Meld::Chi){
        if(meld.start.getSuit() == Piece::BAMBOO_SUIT){
          bambooChi[meld.start.getPieceNum()] = true;
        }
        if(meld.start.getSuit() == Piece::CHARACTER_SUIT){
          charChi[meld.start.getPieceNum()] = true;
        }
        if(meld.start.getSuit() == Piece::PIN_SUIT){
          pinChi[meld.start.getPieceNum()] = true;
        }
      }
    }
    for(int i = 0; i < 9; i++){
      if(bambooChi[i] && charChi[i] && pinChi[i]){
        return state.hands[player].open ? 1 : 2;
      }
    }
    return 0;
  }
    

  auto isPureStraight(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    bool bambooChi[3] = {};
    bool charChi[3] = {};
    bool pinChi[3] = {};
    for(const auto& node : branch){
      if(node->type == Node::ChiSet){
        int ind;
        if(node->start.getPieceNum() == 1){
          ind = 0;
        }else if(node->start.getPieceNum() == 4){
          ind = 1;
        }else if(node->start.getPieceNum() == 7){
          ind = 2;
        }else{
          continue;
        }
        if(node->start.getSuit() == Piece::BAMBOO_SUIT){
          bambooChi[ind] = true;
        }
        if(node->start.getSuit() == Piece::CHARACTER_SUIT){
          charChi[ind] = true;
        }
        if(node->start.getSuit() == Piece::PIN_SUIT){
          pinChi[ind] = true;
        }
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type == Meld::Chi){
        int ind;
        if(meld.start.getPieceNum() == 1){
          ind = 0;
        }else if(meld.start.getPieceNum() == 4){
          ind = 1;
        }else if(meld.start.getPieceNum() == 7){
          ind = 2;
        }else{
          continue;
        }
        if(meld.start.getSuit() == Piece::BAMBOO_SUIT){
          bambooChi[ind] = true;
        }
        if(meld.start.getSuit() == Piece::CHARACTER_SUIT){
          charChi[ind] = true;
        }
        if(meld.start.getSuit() == Piece::PIN_SUIT){
          pinChi[ind] = true;
        }
      }
    }
    int han = state.hands[player].open ? 1 : 2;
    if(bambooChi[0],bambooChi[1],bambooChi[2]){
      return han;
    }
    if(charChi[0],charChi[1],charChi[2]){
      return han;
    }
    if(pinChi[0],pinChi[1],pinChi[2]){
      return han;
    }
    return 0;
  }

  //this wont work please change
  auto isWhiteDragonPon(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int cnt = countPiece(state, player, Piece::WHITE_DRAGON);
    if(cnt == 4){
      return 1;
    }
    return 0;
  }

  auto isGreenDragonPon(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int cnt = countPiece(state, player, Piece::GREEN_DRAGON);
    if(cnt == 4){
      return 1;
    }
    return 0;
  }

  auto isRedDragonPon(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int cnt = countPiece(state, player, Piece::RED_DRAGON);
    if(cnt == 4){
      return 1;
    }
    return 0;
  }

  auto isSeatWind(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int cnt = countPiece(state, player, Piece::fromWind(GetSeat(state.roundNum,player)));
    if(cnt == 3){
      return 1;
    }
    return 0;
  }

  auto isPrevalentWind(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int cnt = countPiece(state, player, Piece::fromWind(state.roundNum > 3 ? South : East));
    if(cnt == 3){
      return 1;
    }
    return 0;
  }

  auto isOutsideHand(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    bool chi = false;
    for(const auto& node : branch){
      if(node->type == Node::ChiSet){
        if(node->start.getPieceNum() == 1 ||
           node->start.getPieceNum() == 7)
        {
          chi = true;
        }else{
          return 0;
        }
      }else{
        if(node->start.getPieceNum() != 1 &&
           node->start.getPieceNum() != 9 &&
          !node->start.isHonor())
        {
          return 0;
        }
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type == Meld::Chi){
        if(meld.start.getPieceNum() == 1 ||
           meld.start.getPieceNum() == 7)
        {
          chi = true;
        }else{
          return 0;
        }
      }else{
        if(meld.start.getPieceNum() != 1 &&
           meld.start.getPieceNum() != 9 &&
          !meld.start.isHonor())
        {
          return 0;
        }
      }
    }
    if(!chi){
      return 0;
    }
    return state.hands[state.currentPlayer].open ? 1:2;
  }

  auto isAfterAKan(const GameState& state, int, const std::vector<const Node*>) -> int{
    if(state.nextState == KanDiscard){
      return 1;
    }
    return 0;
  }

  auto isRobbingAKan(const GameState& state, int, const std::vector<const Node*>) -> int{
    if(state.nextState == KanDiscard){
      return 1;
    }
    return 0;
  }

  auto isBottomOfTheSea(const GameState& state, int, const std::vector<const Node*>) -> int{
    if(state.walls.GetRemainingPieces() == 0){
      return 1;
    }
    return 0;
  }

  auto isSevenPairs(const GameState& state, int player, const std::vector<const Node*>) -> int{
    if(state.hands[player].open){
      return 0;
    }
    for(size_t i = 0; i < state.hands[player].live.size(); i += 2){
      if(state.hands[player].live[i] != state.hands[player].live[i+1]){
        return 0;
      }
    }
    return 2;
  }

  auto isTriplePon(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    bool bambooPon[9] = {};
    bool charPon[9] = {};
    bool pinPon[9] = {};
    for(const auto& node : branch){
      if(node->type == Node::PonSet){
        if(node->start.getSuit() == Piece::BAMBOO_SUIT){
          bambooPon[node->start.getPieceNum()] = true;
        }
        if(node->start.getSuit() == Piece::CHARACTER_SUIT){
          charPon[node->start.getPieceNum()] = true;
        }
        if(node->start.getSuit() == Piece::PIN_SUIT){
          pinPon[node->start.getPieceNum()] = true;
        }
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type == Meld::Pon){
        if(meld.start.getSuit() == Piece::BAMBOO_SUIT){
          bambooPon[meld.start.getPieceNum()] = true;
        }
        if(meld.start.getSuit() == Piece::CHARACTER_SUIT){
          charPon[meld.start.getPieceNum()] = true;
        }
        if(meld.start.getSuit() == Piece::PIN_SUIT){
          pinPon[meld.start.getPieceNum()] = true;
        }
      }
    }
    for(int i = 0; i < 9; i++){
      if(bambooPon[i] && charPon[i] && pinPon[i]){
        return 2;
      }
    }
    return 0;
  }

  auto isThreeConcealedPons(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    int concealedPons = 0;
    for(const auto& node : branch){
      if(node->type == Node::PonSet){
        concealedPons++;
      }
    }
    if(concealedPons >= 3){
      return 2;
    }
    return 0;
  }

  auto isThreeKans(const GameState& state, int player, const std::vector<const Node*> branch){

  }

  auto isAllPons(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    for(const auto& node : branch){
      if(node->type != Node::PonSet){
        return 0;
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type != Meld::Pon){
        return 0;
      }
    }
    return 2;
  }

  auto isHalfFlush(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
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
        return 0;
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
        return 0;
      }
    }
    if(honors){
      return state.hands[player].open ? 2 : 3;
    }
    return 0;
  }

  // auto isLittleThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isAllTerminalsAndHonors(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isTerminalsInAllSets(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isTwicePureDoubleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isBlessingOfMan(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isFullFlush(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  auto isThirteenOrphans(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(state.hands[player].open){
      return 0;
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
      return 1;
    }
    return 0;
  }

  // auto isNineGates(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isBlessingOfHeaven(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isBlessingOfEarth(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isFourConcealedPon(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
  //   if(state.hands[player].open){
  //     return DoesNotSatisfy;
  //   }
  // }

  // auto isFourKans(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isAllGreen(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isAllTerminals(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isAllHonors(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isBigThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isLittleFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

  // auto isBigFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    
  // }

};
