#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"

TEST(CountPieces, CountPieces) {
  GameState state;
  state.hands[0].live = {ONE_CHARACTER,FOUR_PIN,WHITE_DRAGON,WHITE_DRAGON};

  EXPECT_EQ(MahjongGameManager::CountPieces(state,0,ONE_CHARACTER),1);
  EXPECT_EQ(MahjongGameManager::CountPieces(state,0,FOUR_PIN),1);
  EXPECT_EQ(MahjongGameManager::CountPieces(state,0,WHITE_DRAGON),2);
  EXPECT_EQ(MahjongGameManager::CountPieces(state,0,GREEN_DRAGON),0);
}
