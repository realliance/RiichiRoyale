#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"

TEST(RemovePieces, RemoveCorrectAmount) {
  GameState state;
  state.hands[0].live = {ONE_CHARACTER,FOUR_PIN,WHITE_DRAGON,WHITE_DRAGON};

  EXPECT_EQ(MahjongGameManager::RemovePieces(state,0,ONE_CHARACTER,1),1);
  EXPECT_EQ(state.hands[0].live.size(),3);
}

TEST(RemovePieces, RemoveOnlyTheOnesAvailable) {
  GameState state;
  state.hands[0].live = {ONE_CHARACTER,FOUR_PIN,WHITE_DRAGON,WHITE_DRAGON};

  EXPECT_EQ(MahjongGameManager::RemovePieces(state,0,FOUR_PIN,2),1);
  EXPECT_EQ(state.hands[0].live.size(),3);
}

TEST(RemovePieces, OnlyRemovesOne) {
  GameState state;
  state.hands[0].live = {ONE_CHARACTER,FOUR_PIN,WHITE_DRAGON,WHITE_DRAGON};

  EXPECT_EQ(MahjongGameManager::RemovePieces(state,0,WHITE_DRAGON,1),1);
  EXPECT_EQ(state.hands[0].live.back(),WHITE_DRAGON);
}

TEST(RemovePieces, RemovesZeroWhenPieceDoesntExist) {
  GameState state;
  state.hands[0].live = {ONE_CHARACTER,FOUR_PIN,WHITE_DRAGON,WHITE_DRAGON};

  EXPECT_EQ(MahjongGameManager::RemovePieces(state,0,GREEN_DRAGON,8),0);
  EXPECT_EQ(state.hands[0].live.size(),4);
}
