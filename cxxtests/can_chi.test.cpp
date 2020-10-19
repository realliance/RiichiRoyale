#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"


TEST(CanChi, EastShouldChiOnNorth) {
  GameState state;
  state.currentState = AfterDiscard;
  state.lastDiscard = ONE_CHARACTER;
  state.currentPlayer = 3;
  state.hands[0].live= {ONE_CHARACTER,TWO_CHARACTER,THREE_CHARACTER};
  EXPECT_TRUE(MahjongGameManager::CanChi(state,0));
  state.lastDiscard = TWO_CHARACTER;
  EXPECT_TRUE(MahjongGameManager::CanChi(state,0));
  state.lastDiscard = THREE_CHARACTER;
  EXPECT_TRUE(MahjongGameManager::CanChi(state,0));
  state.lastDiscard = FOUR_CHARACTER;
  EXPECT_TRUE(MahjongGameManager::CanChi(state,0));
}

TEST(CanChi, EastShouldNotChiOnSouth) {
  GameState state;
  state.currentState = AfterDiscard;
  state.lastDiscard = ONE_CHARACTER;
  state.currentPlayer = 1;
  state.hands[0].live= {ONE_CHARACTER,TWO_CHARACTER,THREE_CHARACTER};
  EXPECT_FALSE(MahjongGameManager::CanChi(state,0));
}

TEST(CanChi, NoChiAfterKan) {
  GameState state;
  state.currentState = AfterConcealedKanDiscard;
  state.lastDiscard = ONE_CHARACTER;
  state.currentPlayer = 3;
  state.hands[0].live= {ONE_CHARACTER,TWO_CHARACTER,THREE_CHARACTER};
  EXPECT_FALSE(MahjongGameManager::CanChi(state,0));
  state.currentState = AfterKanDiscard;
  EXPECT_FALSE(MahjongGameManager::CanChi(state,0));
}

TEST(CanChi, NoChiOnHonor) {
  GameState state;
  state.currentState = AfterDiscard;
  state.lastDiscard = ONE_CHARACTER;
  state.currentPlayer = 3;
  state.hands[0].live= {ONE_CHARACTER,TWO_CHARACTER,THREE_CHARACTER};
  state.lastDiscard = WHITE_DRAGON;
  EXPECT_FALSE(MahjongGameManager::CanChi(state,0));
}
