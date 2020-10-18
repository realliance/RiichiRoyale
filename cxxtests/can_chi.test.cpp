#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"


TEST(CanChi, ShouldChi_) {
  GameState state;
  state.currentState = AfterDiscard;
  state.lastDiscard = ONE_CHARACTER;
  state.currentPlayer = 3;
  state.hands[0].live= {ONE_CHARACTER,ONE_CHARACTER,ONE_CHARACTER};
  EXPECT_TRUE(MahjongGameManager::CanChi(state,0));
}
