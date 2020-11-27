#include "totobot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind
#include <iostream>     // for endl, ostream, basic_ostream<>::__ostream_type
#include <memory>       // for allocator_traits<>::value_type

auto TotoBot::Name() -> std::string{
  return "TotoBot";
}

auto TotoBot::GameStart(int) -> void {}

auto TotoBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind, Mahjong::Wind) -> void {
  hand = _hand;
  lastEvent.type = Mahjong::Event::Discard;
}

auto TotoBot::ReceiveEvent(Mahjong::Event e) -> void{
  if(e.decision){
    if(e.type <= lastEvent.type)
    lastEvent = e;
  }else if(e.type == Mahjong::Event::Discard){
    hand.push_back(e.piece);
  }
}

auto TotoBot::RetrieveDecision() -> Mahjong::Event{
  if(lastEvent.type != Mahjong::Event::Discard){
    lastEvent.type = Mahjong::Event::Decline;
  }
  Mahjong::Event e = lastEvent;
  lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
  return e;
}
