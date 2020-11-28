#pragma once
#include "piecetype.h"
#include "handnode.h"


namespace Mahjong {
  struct GameState;

  struct score {
    int yakuman = 0;
    int han = 0;
  };

  auto scoreHand(const GameState& state, int player) -> bool;

  auto countPiece(const GameState& state, int player, Piece p) -> int;

  auto isComplete(const GameState& state, int player) -> bool;

  auto isInTenpai(std::vector<Piece> hand, bool allWaits = false) -> std::vector<Piece>;

  using yakuFunc = int(*)(const GameState&, int, const std::vector<const Node*>);

  auto isRiichi(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isFullyConcealedHand(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isPinfu(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isPureDoubleChi(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isAllSimples(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isMixedTripleChi(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isPureStraight(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isWhiteDragonPon(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isGreenDragonPon(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isRedDragonPon(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isSeatWind(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isPrevalentWind(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isOutsideHand(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isAfterAKan(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isRobbingAKan(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isBottomOfTheSea(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isSevenPairs(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;

  auto isTriplePon(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isThreeConcealedPons(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isThreeKans(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isAllPons(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isHalfFlush(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isLittleThreeDragons(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isAllTerminalsAndHonors(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isTerminalsInAllSets(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isTwicePureDoubleChi(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isBlessingOfMan(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isFullFlush(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isThirteenOrphans(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;

  auto isNineGates(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isBlessingOfHeaven(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isBlessingOfEarth(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isFourConcealedPon(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isFourKans(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isAllGreen(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isAllTerminals(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isAllHonors(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isBigThreeDragons(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isLittleFourWinds(const GameState& state, int player, const std::vector<const Node*>) -> int;

  auto isBigFourWinds(const GameState& state, int player, const std::vector<const Node*>) -> int;

};
