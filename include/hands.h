#pragma once
#include "piecetype.h"
#include "score.h"
#include "handnode.h"


namespace Mahjong {
  struct GameState;



  auto scoreHand(const GameState& state, int player) -> Score;

  auto getBasicPoints(Score s) -> int;

  auto getFu(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isOpenPinfu(const GameState& state, int player, const std::vector<const Node*> branch) -> bool;
  
  auto countPiece(const GameState& state, int player, Piece p) -> int;

  auto isComplete(const GameState& state, int player) -> bool;

  auto isInTenpai(std::vector<Piece> hand, bool allWaits = false) -> std::vector<Piece>;

  auto getRiichiDiscard(std::vector<Piece> hand) -> std::vector<Piece>;

  using yakuFunc = int(*)(const GameState&, int, const  std::vector<const Node*>);

  auto isRiichi(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isFullyConcealedHand(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isPinfu(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isPureDoubleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isAllSimples(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isMixedTripleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isPureStraight(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isWindOrDragonPon(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isOutsideHand(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isAfterAKan(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isRobbingAKan(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isBottomOfTheSea(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isSevenPairs(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;

  auto isTriplePon(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isThreeConcealedPons(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isThreeKans(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isAllPons(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isHalfFlush(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isLittleThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isAllTerminalsAndHonors(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isTerminalsInAllSets(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isTwicePureDoubleChi(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isBlessingOfMan(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isFullFlush(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isThirteenOrphans(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;

  auto isNineGates(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isBlessingOfHeaven(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isBlessingOfEarth(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isFourConcealedPon(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isFourKans(const GameState& state, int player, const  std::vector<const Node*> = {}) -> int;

  auto isAllGreen(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;

  auto isAllTerminals(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;

  auto isAllHonors(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;

  auto isBigThreeDragons(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isLittleFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isBigFourWinds(const GameState& state, int player, const std::vector<const Node*> branch) -> int;

  auto isMaxBranches(const GameState& state, int player, const std::vector<const Node*> = {}) -> int;
};
