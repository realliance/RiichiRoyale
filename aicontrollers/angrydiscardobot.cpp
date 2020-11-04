#include "angrydiscardobot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
      // for Wind

auto AngryDiscardoBot::Name() -> std::string{
  return "AngryDiscardoBot";
}

auto AngryDiscardoBot::GameStart(int _playerID) -> void{}

auto AngryDiscardoBot::RoundStart(std::vector<Mahjong::Piece> board, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void{}

auto AngryDiscardoBot::ReceiveEvent(Mahjong::Event e) -> void{
  if(e.decision){
    lastEvent = e;
  }
}

auto AngryDiscardoBot::RetrieveDecision() -> Mahjong::Event{
  if(lastEvent.type < Mahjong::Event::Decline){
    lastEvent.type = Mahjong::Event::Decline;
  }
  return lastEvent;
}
