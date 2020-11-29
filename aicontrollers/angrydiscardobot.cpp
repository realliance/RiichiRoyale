#include "angrydiscardobot.h"
#include "event.h"
#include "piecetype.h"
#include "winds.h"
#include <iostream>
#include <memory>

auto AngryDiscardoBot::Name() -> std::string{
  return "AngryDiscardoBot";
}

auto AngryDiscardoBot::GameStart(int) -> void {}

auto AngryDiscardoBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind, Mahjong::Wind) -> void {
  hand = _hand;
  lastEvent.type = Mahjong::Event::Discard;
}

auto AngryDiscardoBot::ReceiveEvent(Mahjong::Event e) -> void{
  if(e.decision){
    if(e.type <= lastEvent.type)
    lastEvent = e;
  }else if(e.type == Mahjong::Event::Discard){
    hand.push_back(e.piece);
  }
}

auto AngryDiscardoBot::RetrieveDecision() -> Mahjong::Event{
  if(lastEvent.type == Mahjong::Event::Discard){
    lastEvent.piece = hand[n].toUint8_t();
    n = (n+1)%hand.size();
  }
  Mahjong::Event e = lastEvent;
  lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
  return e;
}
