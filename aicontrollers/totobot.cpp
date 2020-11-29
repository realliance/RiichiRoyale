#include "totobot.h"
#include "event.h"
#include "piecetype.h"
#include "winds.h"
#include <iostream>
#include <memory>

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
