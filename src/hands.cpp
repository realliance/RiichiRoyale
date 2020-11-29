#include "hands.h"
#include <stddef.h>
#include <array>
#include <map>
#include <vector>
//#include <ext/alloc_traits.h>
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
  isFullyConcealedHand, isPinfu, isPureDoubleChi, isAllSimples, 
  isMixedTripleChi, isPureStraight, isWindOrDragonPon, isOutsideHand,
  isAfterAKan, isRobbingAKan, isBottomOfTheSea, isSevenPairs,
  isTriplePon, isThreeConcealedPons, isThreeKans,
  isAllPons, isHalfFlush, isLittleThreeDragons, isAllTerminalsAndHonors,
  isTerminalsInAllSets, isTwicePureDoubleChi, isBlessingOfMan, isFullFlush
};

std::vector<yakuFunc> yakumanFunctions = {
  isThirteenOrphans, isNineGates, isBlessingOfHeaven,
  isBlessingOfEarth, isFourConcealedPon, isFourKans,
  isAllGreen, isAllTerminals, isAllHonors, isBigThreeDragons,
  isLittleFourWinds, isBigFourWinds, isMaxBranches
};

namespace Mahjong {

  auto scoreHand(const GameState& state, int player) -> Score{
    const Node* root = breakdownHand(state.hands[state.currentPlayer].live);
    Score s;
    s.han = 0;
    s.yakuman = 0;
    s.fu = 0;
    int basepoints = 0;
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
      if(s.yakuman > 0 || branchyakumans > 0){
        s.yakuman = branchyakumans > s.yakuman ? branchyakumans : s.yakuman;
        continue;
      }
      if(s.han >= 5 || branchhan >= 5){
        s.han = branchhan > s.han ? branchhan : s.han;
        continue;
      }
      int fu = getFu(state,player,branch);
      if(basepoints == 0){
        s.han = branchhan;
        s.fu = fu;
        basepoints = fu*(2<<(1+branchhan));
      }
      int branchbase = fu*(2<<(1+branchhan));
      if(branchbase > basepoints){
        s.han = branchhan;
        s.fu = fu;
        basepoints = branchbase;
      }
    }
    delete root;
    return s;
  }

  auto getBasicPoints(Score s) -> int{
    if(s.yakuman > 0){
      return s.yakuman*8000;
    }
    if(s.han >= 5){
      switch (s.han){
      case 5:
        return 2000;
      case 6:
      case 7:
        return 3000;
      case 8:
      case 9:
      case 10: 
        return 4000;     
      default:
        return 6000;
      }
    }
    int p = s.fu*(2<<(1+s.han));
    if(p > 2000){
      return 2000;
    }
    return p;
  }

  auto getFu(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(isSevenPairs(state,player,branch)){
      return 25;
    }
    if(isPinfu(state,player,branch)){
      if(state.hasRonned[player]){
        return 30;
      }
      return 20;
    }
    int fu;
    if(!state.hands[player].open && state.hasRonned[player]){
      fu = 25;
    }else{
      fu = 20;
    }
    bool open = state.hands[player].open;
    if(isOpenPinfu(state,player,branch)){
      fu += 2;
    }
    for(const auto& node : branch){
      if(node->type == Node::PonSet){
        if(!node->start.isHonor() && !node->start.isTerminal()){
          fu += open ? 2 : 4;
        }else{
          fu += open ? 4 : 8;
        }
      }
      if(open){
        continue;
      }
      if(node->type == Node::Pair){
        if(node->start.isHonor()){
          if(node->start == Piece::GREEN_DRAGON ||
             node->start == Piece::RED_DRAGON ||
             node->start == Piece::WHITE_DRAGON ||
             state.roundNum > 3 ? Piece::SOUTH_WIND : Piece::EAST_WIND
          ){
            fu += 2;
          }
        }
        if(node->start == state.pendingPiece){
          fu += 2;
        }
      }
      if(node->type == Node::ChiSet){
        if(node->start+1 == state.pendingPiece){
          fu += 2;
        }
        if(node->start.getPieceNum() == 1 && state.pendingPiece.getPieceNum() == 3 &&
           node->start.getSuit() == state.pendingPiece.getSuit())
        {
          fu += 2;
        }
        if(node->start.getPieceNum() == 7 && state.pendingPiece == node->start)
        {
          fu += 2;
        }
      }
    }
    if(fu%10){
      return fu;
    }
    return fu+(10-(fu%10)); //rounding up to multiple of ten
  }

    auto isOpenPinfu(const GameState& state, int player, const std::vector<const Node*> branch) -> bool{
    for(const auto& node : branch){
      if(node->type == Node::PonSet){
        return false;
      }
      if(node->type == Node::Pair){
        if(node->start == Piece::RED_DRAGON || 
           node->start == Piece::WHITE_DRAGON || 
           node->start == Piece::GREEN_DRAGON)
        {
          return false;
        }
        if(node->start == Piece::SOUTH_WIND && state.roundNum > 3){
          return false;
        }
        if(node->start == Piece::EAST_WIND && state.roundNum < 4){
          return false;
        }
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type > Meld::Chi){
        return false;
      }
    }
    auto waits = isInTenpai(state.hands[player].live,true);
    if(waits.size() == 1){
      return false;
    }
    return true;
  }

  auto isComplete(const GameState& state, int player) -> bool{
    const Node* root = breakdownHand(state.hands[player].live);
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
  auto isInTenpai(std::vector<Piece> hand, bool allWaits) -> std::vector<Piece> {
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
            delete root;
            return waits;
          }
        }
        delete root;
        hand.pop_back();
      }
      hand.push_back(removed);
    }
    return waits;
  }

  //see above comment
  auto getRiichiDiscard(std::vector<Piece> hand) -> std::vector<Piece> {
    int8_t counts[256] = {};
    bool removedbefore[256] = {};
    std::vector<Piece> removeMe;
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
          removeMe.push_back(removed);
        }
        delete root;
        hand.pop_back();
      }
      hand.push_back(removed);
    }
    return removeMe;
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
    if(state.currentPlayer == player && !state.hands[player].open && state.walls.GetRemainingPieces() > 0){
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
    if(isTwicePureDoubleChi(state,player,branch)){
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
        if(branch[i]->type == branch[j]->type && branch[i]->start == branch[j]->start){
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
    if(bambooChi[0] && bambooChi[1] && bambooChi[2]){
      return han;
    }
    if(charChi[0] && charChi[1] && charChi[2]){
      return han;
    }
    if(pinChi[0] && pinChi[1] && pinChi[2]){
      return han;
    }
    return 0;
  }

  auto isWindOrDragonPon(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    std::vector<Piece> matches {
      Piece::WHITE_DRAGON, Piece::GREEN_DRAGON, Piece::RED_DRAGON, 
      Piece::fromWind(GetSeat(state.roundNum,player)),
      Piece::fromWind(state.roundNum > 3 ? South : East)
    };
    int han = 0;
    for(const auto& match: matches){
      bool isMatch = false;
      for(const auto& node : branch){
        if(node->type == Node::PonSet && node->start == match){
          isMatch = true;
          break;
        }
      }
      for(const auto& meld : state.hands[player].melds){
        if(meld.type >= Meld::Pon && meld.start == match){
          isMatch = true;
          break;
        }
      }
      if(isMatch){
        han++;
      }
    }
    return han;
  }

  auto isOutsideHand(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(isTerminalsInAllSets(state,player,branch) || isAllTerminalsAndHonors(state,player,branch)){
      return 0;
    }
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
          bambooPon[node->start.getPieceNum()-1] = true;
        }
        if(node->start.getSuit() == Piece::CHARACTER_SUIT){
          charPon[node->start.getPieceNum()-1] = true;
        }
        if(node->start.getSuit() == Piece::PIN_SUIT){
          pinPon[node->start.getPieceNum()-1] = true;
        }
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type >= Meld::Pon){
        if(meld.start.getSuit() == Piece::BAMBOO_SUIT){
          bambooPon[meld.start.getPieceNum()-1] = true;
        }
        if(meld.start.getSuit() == Piece::CHARACTER_SUIT){
          charPon[meld.start.getPieceNum()-1] = true;
        }
        if(meld.start.getSuit() == Piece::PIN_SUIT){
          pinPon[meld.start.getPieceNum()-1] = true;
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
    for(const auto& meld : state.hands[player].melds){
      if(meld.type == Meld::ConcealedKan){
        concealedPons++;
      }
    }
    if(concealedPons >= 3){
      return 2;
    }
    return 0;
  }

  auto isThreeKans(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int kans = 0;
    for(const auto& meld : state.hands[player].melds){
      if(meld.type >= Meld::Kan){
        kans++;
      }
    }
    if(kans >= 3){
      return 2;
    }
    return 0;
  }

  auto isAllPons(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(isAllTerminalsAndHonors(state,player,branch)){
      return 0;
    }
    for(const auto& node : branch){
      if(node->type != Node::PonSet){
        return 0;
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type == Meld::Chi){
        return 0;
      }
    }
    return 2;
  }

  auto isHalfFlush(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(isFullFlush(state,player,branch)){
      return 0;
    }
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

  auto isLittleThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    bool pair = false;
    int pons = 0;
    bool red = false;
    bool green = false;
    bool white = false;
    for(const auto& node : branch){
      if(node->start == Piece::RED_DRAGON){
        red = true;
      }else if(node->start == Piece::GREEN_DRAGON){
        green = true;
      }else if(node->start == Piece::WHITE_DRAGON){
        white = true;
      }else{
        continue;
      }
      if(node->type != Node::Pair){
        pons++;
      }else{
        if(pair){
          return 0;
        }else{
          pair = true;
        }
      }
    }
    if(!pair){
      return 0;
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.start == Piece::RED_DRAGON){
        red = true;
      }else if(meld.start == Piece::GREEN_DRAGON){
        green = true;
      }else if(meld.start == Piece::WHITE_DRAGON){
        white = true;
      }else{
        continue;
      }
      pons++;
    }
    if(pons == 2 && pair){
      return 2;
    }
    return 0;
  }

  auto isAllTerminalsAndHonors(const GameState& state, int player, const std::vector<const Node*>) -> int{
    for(const auto & piece : state.hands[player].live){
      if(!piece.isHonor() && !piece.isTerminal()){
        return 0;
      }
    }
    for(const auto & meld : state.hands[player].melds){
      if(!meld.start.isHonor() && !meld.start.isTerminal()){
        return 0;
      }
    }
    return 2;
  }

  auto isTerminalsInAllSets(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(isFullFlush(state,player,branch)){
      return 0;
    }
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
           node->start.getPieceNum() != 9)
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
           meld.start.getPieceNum() != 9)
        {
          return 0;
        }
      }
    }
    if(!chi){
      return 0;
    }
    return state.hands[state.currentPlayer].open ? 2:3;
  }

  auto isTwicePureDoubleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(state.hands[player].open){
      return 0;
    }
    int twice = 0;
    for(size_t i = 0; i < branch.size(); i++){
      if(branch[i]->type != Node::ChiSet){
        continue;
      }
      for(size_t j = 0; j < branch.size(); j++){
        if(i==j ){
          continue;
        }
        if(branch[i]->type == branch[j]->type && branch[i]->start == branch[j]->start){
          twice++;
        }
      }
    }
    if(twice == 2){
      return 3;
    }
    return 0;
  }

  auto isBlessingOfMan(const GameState& state, int player, const std::vector<const Node*>) -> int{
    if(state.hands[player].open){
      return 0;
    }
    if(state.turnNum > 3){
      return 0;
    }
    if(state.lastCall >= 0){
      return 0;
    }
    if(state.hasRonned[player]){
      return 5;
    }
    return 0;
  }

  auto isFullFlush(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int suit = state.hands[player].live.front().getSuit();
    for(const auto& piece : state.hands[player].live){
      if(piece.getSuit() != suit){
        return 0;
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.start.getSuit() != suit){
        return 0;
      }
    }
    return state.hands[player].open ? 5 : 6;
  }

  auto isThirteenOrphans(const GameState& state, int player, const std::vector<const Node*>) -> int{
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
    if(!duplicate){
      return 0;
    }
    for(const auto& [_,match] : pieces){
      if(!match){
        return 0;
      }
    }
    return 1;
  }

  auto isNineGates(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(state.hands[player].open){
      return 0;
    }
    if(!isFullFlush(state,player,branch)){
      return 0;
    }
        if(state.hands[player].open){
      return 0;
    }
    std::map<int,int> pieces = {
      {1,0},{2,0},{3,0},{4,0},
      {5,0},{6,0},{7,0},{8,0},
      {9,0}
    };
    bool duplicate = false;
    for(const auto & piece : state.hands[player].live){
      if(pieces.contains(piece.getPieceNum())){
        if(pieces[piece.getPieceNum()] && !piece.isTerminal()){
          if(duplicate){
            return false;
          }
          duplicate = true;
        }else{
          pieces[piece.getPieceNum()]++;
        }
      }
    }
    if(!duplicate){
      return false;
    }
    for(const auto& [piece,count] : pieces){
      if((piece == 1 || piece == 9)){
        if(count != 3){
          return 0;
        }
      }else if(count != 1){
        return 0;
      }
    }
    return 1;
  }

  auto isBlessingOfHeaven(const GameState& state, int player, const std::vector<const Node*>) -> int{
    if(state.hands[player].open){
      return 0;
    }
    if(player != 0){
      return 0;
    }
    if(state.hasRonned[0]){
      return 0;
    }
    if(state.turnNum != 0){
      return 0;
    }
    return 1;
  }

  auto isBlessingOfEarth(const GameState& state, int player, const std::vector<const Node*>) -> int{
    if(state.hands[player].open){
      return 0;
    }
    if(state.turnNum > 3){
      return 0;
    }
    if(state.lastCall >= 0){
      return 0;
    }
    if(state.hasRonned[player]){
      return 0;
    }
    return 1;
  }

  auto isFourConcealedPon(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    if(state.hands[player].open){
      return 0;
    }
    int concealedPons = 0;
    for(const auto& node : branch){
      if(node->type == Node::PonSet){
        concealedPons++;
      }
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.type == Meld::ConcealedKan){
        concealedPons++;
      }
    }
    if(concealedPons == 4){
      return 1;
    }
    return 0;
  }

  auto isFourKans(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int kans = 0;
    for(const auto& meld : state.hands[player].melds){
      if(meld.type >= Meld::Kan){
        kans++;
      }
    }
    if(kans == 4){
      return 1;
    }
    return 0;
  }

  auto isAllGreen(const GameState& state, int player, const std::vector<const Node*>) -> int{
    for(const auto & piece : state.hands[player].live){
      if(!piece.isGreen()){
        return 0;
      }
    }
    for(const auto & meld : state.hands[player].melds){
      if(!meld.start.isGreen()){
        return 0;
      }
    }
    return 1;
  }

  auto isAllTerminals(const GameState& state, int player, const std::vector<const Node*>) -> int{
    for(const auto & piece : state.hands[player].live){
      if(!piece.isTerminal()){
        return 0;
      }
    }
    for(const auto & meld : state.hands[player].melds){
      if(!meld.start.isTerminal()){
        return 0;
      }
    }
    return 1;
  }

  auto isAllHonors(const GameState& state, int player, const std::vector<const Node*>) -> int{
    for(const auto & piece : state.hands[player].live){
      if(!piece.isHonor()){
        return 0;
      }
    }
    for(const auto & meld : state.hands[player].melds){
      if(!meld.start.isHonor()){
        return 0;
      }
    }
    return 1;
  }

  auto isBigThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    int pons = 0;
    bool red = false;
    bool green = false;
    bool white = false;
    for(const auto& node : branch){
      if(node->start == Piece::RED_DRAGON){
        red = true;
      }else if(node->start == Piece::GREEN_DRAGON){
        green = true;
      }else if(node->start == Piece::WHITE_DRAGON){
        white = true;
      }else{
        continue;
      }
      if(node->type == Node::Pair){
        return 0;
      }
      pons++;
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.start == Piece::RED_DRAGON){
        red = true;
      }else if(meld.start == Piece::GREEN_DRAGON){
        green = true;
      }else if(meld.start == Piece::WHITE_DRAGON){
        white = true;
      }else{
        continue;
      }
      pons++;
    }
    if(pons == 3){
      return 1;
    }
    return 0;
  }

  auto isLittleFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    bool pair = false;
    int pons = 0;
    bool east = false;
    bool south = false;
    bool west = false;
    bool north = false;
    for(const auto& node : branch){
      if(node->start == Piece::EAST_WIND){
        east = true;
      }else if(node->start == Piece::SOUTH_WIND){
        south = true;
      }else if(node->start == Piece::WEST_WIND){
        west = true;
      }else if(node->start == Piece::NORTH_WIND){
        north = true;
      }else{
        continue;
      }
      if(node->type != Node::Pair){
        pons++;
      }else{
        if(pair){
          return 0;
        }else{
          pair = true;
        }
      }
    }
    if(!pair){
      return 0;
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.start == Piece::EAST_WIND){
        east = true;
      }else if(meld.start == Piece::SOUTH_WIND){
        south = true;
      }else if(meld.start == Piece::WEST_WIND){
        west = true;
      }else if(meld.start == Piece::NORTH_WIND){
        north = true;
      }else{
        continue;
      }
      pons++;
    }
    if(pons == 3 && pair){
      return 1;
    }
    return 0;
  }

  auto isBigFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> int{
    int pons = 0;
    bool east = false;
    bool south = false;
    bool west = false;
    bool north = false;
    for(const auto& node : branch){
      if(node->start == Piece::EAST_WIND){
        east = true;
      }else if(node->start == Piece::SOUTH_WIND){
        south = true;
      }else if(node->start == Piece::WEST_WIND){
        west = true;
      }else if(node->start == Piece::NORTH_WIND){
        north = true;
      }else{
        continue;
      }
      if(node->type == Node::Pair){
        return 0;
      }
      pons++;
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.start == Piece::EAST_WIND){
        east = true;
      }else if(meld.start == Piece::SOUTH_WIND){
        south = true;
      }else if(meld.start == Piece::WEST_WIND){
        west = true;
      }else if(meld.start == Piece::NORTH_WIND){
        north = true;
      }else{
        continue;
      }
      pons++;
    }
    if(pons == 4){
      return 1;
    }
    return 0;
  }

  auto isMaxBranches(const GameState& state, int player, const std::vector<const Node*>) -> int{
    int start = state.hands[player].live[0].getPieceNum();
    int suit = state.hands[player].live[0].getSuit();
    int sets[6] = {};
    int final[6] = {3,3,2,2,2,2};
    for(const auto& piece : state.hands[player].live){
      if(piece.getSuit() != suit){
        return 0;
      }
      if(piece.getPieceNum() < start || piece.getPieceNum() > start+5){
        return 0;
      }
      sets[piece.getPieceNum()-start]++;
    }
    for(const auto& meld : state.hands[player].melds){
      if(meld.start.getSuit() != suit){
        return 0;
      }
      if(meld.start.getPieceNum() < start || meld.start.getPieceNum() > start+3){
        return 0;
      }
      if(meld.type >= Meld::Kan){
        if(meld.start.getPieceNum() != start && meld.start.getPieceNum() != start+1){
          return 0;
        }
        sets[meld.start.getPieceNum()-start] += 3;
      }
      if(meld.type == Meld::Pon){
        sets[meld.start.getPieceNum()-start] += 3;
      }
      if(meld.type == Meld::Chi){
        sets[meld.start.getPieceNum()-start]++;
        sets[meld.start.getPieceNum()-start+1]++;
        sets[meld.start.getPieceNum()-start+2]++;
      }
    }
    for(int i = 0; i < 6; i++){
      if(sets[i] != final[i]){
        return 0;
      }
    }
    return 1;
  }

};
