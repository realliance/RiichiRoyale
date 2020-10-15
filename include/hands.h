#pragma once

class RoundState;

enum HandState{
  DoesNotSatisfy, //Doesn't satisfy the yaku
  Tenpai, //Is one piece from a yaku
  Complete, //Has at least one yaku
  Satisfies 
  /* 
   * satisfies the yaku
   * but might need more pieces to be in tenpai/complete
   * Check isInAValidFormat() for Tenpai/Complete
   */
};

auto hasAtLeast(const RoundState* state, Piece p, int num) -> bool;

auto isInStdForm(const RoundState* state) -> HandState;

auto isInAValidFormat(const RoundState* state) -> HandState;

auto isRiichi(const RoundState* state) -> HandState;

auto isFullyConcealedHand(const RoundState* state) -> HandState;

auto isPinfu(const RoundState* state) -> HandState;

auto isPureDoubleChi(const RoundState* state) -> HandState;

auto isAllSimples(const RoundState* state) -> HandState;

auto isMixedTripleChi(const RoundState* state) -> HandState;

auto isPureStraight(const RoundState* state) -> HandState;

auto isWhiteDragonPon(const RoundState* state) -> HandState;

auto isGreenDragonPon(const RoundState* state) -> HandState;

auto isRedDragonPon(const RoundState* state) -> HandState;

auto isSeatWind(const RoundState* state) -> HandState;

auto isPrevalentWind(const RoundState* state) -> HandState;

auto isOutsideHand(const RoundState* state) -> HandState;

auto isAfterAKon(const RoundState* state) -> HandState;

auto isRobbingAKong(const RoundState* state) -> HandState;

auto isBottomOfTheSea(const RoundState* state) -> HandState;

auto isSevenPairs(const RoundState* state) -> HandState;

auto isTriplePon(const RoundState* state) -> HandState;

auto isThreeConcealedPons(const RoundState* state) -> HandState;

auto isAllPons(const RoundState* state) -> HandState;

auto isHalfFlush(const RoundState* state) -> HandState;

auto isLittleThreeDragons(const RoundState* state) -> HandState;

auto isAllTerminalsAndHonors(const RoundState* state) -> HandState;

auto isTerminalsInAllSets(const RoundState* state) -> HandState;

auto isTwicePureDoubleChi(const RoundState* state) -> HandState;

auto isBlessingOfMan(const RoundState* state) -> HandState;

auto isFullFlush(const RoundState* state) -> HandState;

auto isThirteenOrphans(const RoundState* state) -> HandState;

auto isNineGates(const RoundState* state) -> HandState;

auto isBlessingOfHeaven(const RoundState* state) -> HandState;

auto isBlessingOfEarth(const RoundState* state) -> HandState;

auto isFourConcealedPon(const RoundState* state) -> HandState;

auto isFourKans(const RoundState* state) -> HandState;

auto isAllGreen(const RoundState* state) -> HandState;

auto isAllTerminals(const RoundState* state) -> HandState;

auto isAllHonors(const RoundState* state) -> HandState;

auto isBigThreeDragons(const RoundState* state) -> HandState;

auto isLittleFourWinds(const RoundState* state) -> HandState;

auto isBigFourWinds(const RoundState* state) -> HandState;
