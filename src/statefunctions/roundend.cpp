#include <array>
#include "event.h"
#include "gamestate.h"
#include "piecetype.h"
#include "statefunctions.h"
#include "stateutilities.h"
using namespace Mahjong;

auto Mahjong::RoundEnd(GameState& state) -> GameState& {  
  state.currentPlayer = -1;
  state.turnNum = -1;
  state.lastCall = -1;
  state.concealedKan = false;
  state.lastCaller = -1;
  state.pendingPiece = Piece::ERROR;
  state.hasRonned = {};
  
  state.hands = {};

  const int LAST_ROUND = 3;
  if(state.roundNum > LAST_ROUND && state.riichiSticks > 0){
    std::vector<int> winners;
    int highscore = -100000;
    for(int i = 0; i < 4; i++){
      if(state.players[i].points+state.scores[i] > highscore){
        highscore = state.players[i].points+state.scores[i];
        winners.clear();
        winners.push_back(i);
      }else if(state.players[i].points+state.scores[i] == highscore){
        winners.push_back(i);
      }
    }
    for(const auto& winner : winners){
      state.scores[winner] += (state.riichiSticks * 1000)/winners.size();
    }
  }

  //TODO: scoring
  for(int i = 0; i < 4; i ++){
    AlertPlayers(state, Event{
      Event::PointDiff,
      i,
      static_cast<int16_t>(state.scores[i]/100),
      false
    });
    state.players[i].points += state.scores[i];
  }
  state.scores = {};

  if(state.roundNum > LAST_ROUND){ // east only lmao
    state.nextState = GameEnd;
  }else{
    state.nextState = RoundStart;
  }
  return state;
}
