#pragma once
#include "piecetype.h"


namespace Mahjong {
  struct GameState;

  //bleh rewriting soon
  //this comment might come back to haunt me 10/31/2020
  enum HandState{
    DoesNotSatisfy, //Doesn't satisfy the yaku
    Tenpai, //Is one piece from a yaku
    Complete, //Is in a valid form, might have no yaku
    Satisfies 
    /* 
    * satisfies the yaku
    * but might need more pieces to be in tenpai/complete
    * Check isInAValidFormat() for Tenpai/Complete
    */
  };

  auto countPiece(const GameState& state, int player, Piece p) -> int;

  auto isComplete(const GameState& state, int player) -> bool;

  auto isInStdForm(const GameState& state, int player) -> HandState;

  auto isInAValidFormat(const GameState& state, int player) -> HandState;

  // auto isRiichi(const GameState& state, int player) -> HandState;

  auto isFullyConcealedHand(const GameState& state, int player) -> HandState;

  // auto isPinfu(const GameState& state, int player) -> HandState;

  // auto isPureDoubleChi(const GameState& state, int player) -> HandState;

  auto isAllSimples(const GameState& state, int player) -> HandState;

  // auto isMixedTripleChi(const GameState& state, int player) -> HandState;

  // auto isPureStraight(const GameState& state, int player) -> HandState;

  auto isWhiteDragonPon(const GameState& state, int player) -> HandState;

  auto isGreenDragonPon(const GameState& state, int player) -> HandState;

  auto isRedDragonPon(const GameState& state, int player) -> HandState;

  auto isSeatWind(const GameState& state, int player) -> HandState;

  auto isPrevalentWind(const GameState& state, int player) -> HandState;

  // auto isOutsideHand(const GameState& state, int player) -> HandState;

  auto isAfterAKan(const GameState& state, int player) -> HandState;

  auto isRobbingAKan(const GameState& state, int player) -> HandState;

  auto isBottomOfTheSea(const GameState& state, int player) -> HandState;

  auto isSevenPairs(const GameState& state, int player) -> HandState;

  // auto isTriplePon(const GameState& state, int player) -> HandState;

  // auto isThreeConcealedPons(const GameState& state, int player) -> HandState;

  // auto isAllPons(const GameState& state, int player) -> HandState;

  // auto isHalfFlush(const GameState& state, int player) -> HandState;

  // auto isLittleThreeDragons(const GameState& state, int player) -> HandState;

  // auto isAllTerminalsAndHonors(const GameState& state, int player) -> HandState;

  // auto isTerminalsInAllSets(const GameState& state, int player) -> HandState;

  // auto isTwicePureDoubleChi(const GameState& state, int player) -> HandState;

  // auto isBlessingOfMan(const GameState& state, int player) -> HandState;

  // auto isFullFlush(const GameState& state, int player) -> HandState;

  auto isThirteenOrphans(const GameState& state, int player) -> HandState;

  // auto isNineGates(const GameState& state, int player) -> HandState;

  // auto isBlessingOfHeaven(const GameState& state, int player) -> HandState;

  // auto isBlessingOfEarth(const GameState& state, int player) -> HandState;

  // auto isFourConcealedPon(const GameState& state, int player) -> HandState;

  // auto isFourKans(const GameState& state, int player) -> HandState;

  // auto isAllGreen(const GameState& state, int player) -> HandState;

  // auto isAllTerminals(const GameState& state, int player) -> HandState;

  // auto isAllHonors(const GameState& state, int player) -> HandState;

  // auto isBigThreeDragons(const GameState& state, int player) -> HandState;

  // auto isLittleFourWinds(const GameState& state, int player) -> HandState;

  // auto isBigFourWinds(const GameState& state, int player) -> HandState;

};
