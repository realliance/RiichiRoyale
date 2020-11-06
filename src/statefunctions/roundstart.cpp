#include <stddef.h>
#include <stdint.h>
#include <array>
#include <memory>
#include <vector>
#include "event.h"
#include "gamestate.h"
#include "hand.h"
#include "piecetype.h"
#include "player.h"
#include "playercontroller.h"
#include "statefunctions.h"
#include "stateutilities.h"
#include "walls.h"
#include "winds.h"

using namespace Mahjong;

auto Mahjong::RoundStart(GameState& state) -> GameState& {
  state.walls = Walls();
  for(size_t i = 0; i < 4; i++){
    auto hand = state.walls.TakeHand();
    state.players[i].controller->RoundStart(
      hand,
      static_cast<Wind>((i+3*(state.roundNum%4))%4),
      (state.roundNum > 3) ? South : East
    );
    state.hands[i] = Hand(hand);
  }

  AlertPlayers(state,Event{
    Event::Dora, // type
    -1, // player
    static_cast<int16_t>(state.walls.GetDoras()[0].toUint8_t()), // piece
    false, // decision
  });

  state.nextState = Draw;
  return state;
}
