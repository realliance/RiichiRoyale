#include "angrydiscardobot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind

auto AngryDiscardoBot::Name() -> std::string{
  return "AngryDiscardoBot";
}

auto AngryDiscardoBot::GameStart(int) -> void {}

auto AngryDiscardoBot::RoundStart(std::vector<Mahjong::Piece>, Mahjong::Wind, Mahjong::Wind) -> void {
  lastEvent = Mahjong::END_EVENT;
}

auto AngryDiscardoBot::ReceiveEvent(Mahjong::Event e) -> void{
  if(e.decision){
    if(e.type < lastEvent.type)
    lastEvent = e;
  }
}

auto AngryDiscardoBot::RetrieveDecision() -> Mahjong::Event{
  Mahjong::Event e = lastEvent;
  lastEvent = Mahjong::END_EVENT; // lowest """priority""" event type
  return e;
}
