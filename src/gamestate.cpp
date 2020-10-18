#include "gamestate.h"
#include <stddef.h>  // for size_t

auto operator<<(std::ostream& os, const GameState& state) -> std::ostream&{
  os << "currentPlayer: " << state.currentPlayer << std::endl;
  os << "currentState: " <<  GameStateToStr(state.currentState) << std::endl;
  os << "lastCall: " << state.lastCall << std::endl;
  os << "dealer: " << state.dealer << std::endl;
  os << "lastDiscard: " << state.lastDiscard.toStr() << std::endl;
  os << "prevalentWind: " << WindToStr(state.prevalentWind) << std::endl;
  os << "turnCount: " << state.turnCount << std::endl;
  os << "roundCounter: " << state.roundCounter << std::endl;
  os << "hands: " << std::endl;
  for(size_t i = 0; i < state.hands.size(); i++){
    os << "hand[" << i << "]: {" << state.hands[i] << "}" << std::endl;
  }
  os << "players: " << std::endl;
  for(size_t i = 0; i < state.players.size(); i++){
    os << "player[" << i << "]: " << state.players[i] << std::endl;
  }
  os << "winners: " << std::endl;
  for(size_t i = 0; i < state.winners.size(); i++){
    os << "winner[" << i << "]: " << state.winners[i] << std::endl;
  }
  os << state.walls << std::endl;
  return os;
}
