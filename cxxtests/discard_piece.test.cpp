#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"

TEST(DiscardPiece, DiscardsCorrectPiece) {
  GameState state;
  state.hands[0].live = {ONE_CHARACTER,FOUR_PIN,WHITE_DRAGON,WHITE_DRAGON};

  MahjongGameManager::DiscardPiece(state,0,ONE_CHARACTER);
  EXPECT_EQ(state.hands[0].discards[0],ONE_CHARACTER);
}

TEST(DiscardPiece, DoesntDiscardInCorrectPiece) {
  GameState state;
  state.hands[0].live = {ONE_CHARACTER,FOUR_PIN,WHITE_DRAGON,WHITE_DRAGON};

  MahjongGameManager::DiscardPiece(state,0,GREEN_DRAGON);
  EXPECT_EQ(state.hands[0].live.size(),4);
}
