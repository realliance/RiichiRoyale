#include "angrydiscardobot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind

auto AngryDiscardoBot::Name() -> std::string{
  return "AngryDiscardoBot";
}

auto AngryDiscardoBot::GameStart(int _playerID) -> void{}

auto AngryDiscardoBot::RoundStart(std::vector<Piece> board, Wind seatWind, Wind prevalentWind) -> void{}

auto AngryDiscardoBot::ReceiveEvent(Event e) -> void{
  if(e.decision){
    lastEvent = e;
  }
}

auto AngryDiscardoBot::RetrieveDecision() -> Event{
  if(lastEvent.type < Decline){
    lastEvent.type = Decline;
  }
  return lastEvent;
}
