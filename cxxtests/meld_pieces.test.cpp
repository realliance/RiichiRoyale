#include <gtest/gtest.h>
#include "gamestate.h"
#include "manager.h"

TEST(MeldPieces, EnoughToPon){
  GameState state;
  state.hands[0].live = { TWO_PIN, TWO_PIN, TWO_PIN };
  ASSERT_NO_THROW(MahjongGameManager::MeldPieces(state,0,TWO_PIN,Pon));
  ASSERT_EQ(state.hands[0].melds.size(),1);
  EXPECT_EQ(state.hands[0].melds[0].start, TWO_PIN);
  EXPECT_EQ(state.hands[0].melds[0].type, Pon);
}

TEST(MeldPieces, NotEnoughToPon){
  
}

TEST(MeldPieces, EnoughToChi){
  GameState state;
  state.hands[0].live = { ONE_PIN, TWO_PIN, THREE_PIN };
  ASSERT_NO_THROW(MahjongGameManager::MeldPieces(state,0,ONE_PIN,Chi));
  ASSERT_EQ(state.hands[0].melds.size(),1);
  EXPECT_EQ(state.hands[0].melds[0].start, ONE_PIN);
  EXPECT_EQ(state.hands[0].melds[0].type, ChiMeld);
}

TEST(MeldPieces, NotEnoughToChi){
  GameState state;
  state.hands[0].live = { ONE_PIN, TWO_PIN, THREE_PIN };
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,TWO_PIN,Chi));
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,FOUR_PIN,Chi));
}

TEST(MeldPieces, EnoughToKan){
  GameState state;
  state.hands[0].live = { TWO_PIN, TWO_PIN, TWO_PIN, TWO_PIN };
  ASSERT_NO_THROW(MahjongGameManager::MeldPieces(state,0,TWO_PIN,Kan));
  ASSERT_EQ(state.hands[0].melds.size(),1);
  EXPECT_EQ(state.hands[0].melds[0].start, TWO_PIN);
  EXPECT_EQ(state.hands[0].melds[0].type, KanMeld);
}

TEST(MeldPieces, NotEnoughToKan){
  GameState state;
  state.hands[0].live = { ONE_PIN, TWO_PIN, THREE_PIN };
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,TWO_PIN,Kan));
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,FOUR_PIN,Kan));
}

TEST(MeldPieces, EnoughToConcealedKan){
  GameState state;
  state.hands[0].live = { TWO_PIN, TWO_PIN, TWO_PIN, TWO_PIN };
  ASSERT_NO_THROW(MahjongGameManager::MeldPieces(state,0,TWO_PIN,ConcealedKan));
  ASSERT_EQ(state.hands[0].melds.size(),1);
  EXPECT_EQ(state.hands[0].melds[0].start, TWO_PIN);
  EXPECT_EQ(state.hands[0].melds[0].type, ConcealedKanMeld);
}

TEST(MeldPieces, NotEnoughToConcealedKan){
  GameState state;
  state.hands[0].live = { ONE_PIN, TWO_PIN, THREE_PIN };
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,ONE_PIN,ConcealedKan));
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,FOUR_PIN,ConcealedKan));
}

TEST(MeldPieces, EnoughToConvertedKan){
  GameState state;
  state.hands[0].live = { TWO_PIN };
  state.hands[0].melds.push_back({ PonMeld, TWO_PIN });
  ASSERT_NO_THROW(MahjongGameManager::MeldPieces(state,0,TWO_PIN,ConvertedKan));
  ASSERT_EQ(state.hands[0].melds.size(),1);
  EXPECT_EQ(state.hands[0].melds[0].start, TWO_PIN);
  EXPECT_EQ(state.hands[0].melds[0].type, KanMeld);
}

TEST(MeldPieces, NotEnoughToConvertedKan){
  GameState state;
  state.hands[0].live = { FOUR_PIN };
  state.hands[0].melds.push_back({ PonMeld, TWO_PIN });
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,FOUR_PIN,ConvertedKan));
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,TWO_PIN,ConvertedKan));
  ASSERT_ANY_THROW(MahjongGameManager::MeldPieces(state,0,THREE_PIN,ConvertedKan));
}
