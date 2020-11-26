#include "bolickbot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind
#include <iostream>     // for endl, ostream, basic_ostream<>::__ostream_type
#include <memory>       // for allocator_traits<>::value_type

auto BolickBot::Name() -> std::string
{
  return "bolickBot";
}

auto BolickBot::GameStart(int id) -> void 
{
  pid = id;
}

auto BolickBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind s, Mahjong::Wind p) -> void 
{
  //hand = _hand;
  for(Mahjong::Piece m : _hand)
  {
    handTile h;
    h.piece = m;
    h.weight = 0;
    hand.push_back(h);
  }
  assignweights();
  lastEvent.type = Mahjong::Event::Discard;
  swind = s;
  pwind = p;
}





void BolickBot::assignweights()
{
  
  
  
  
  
}





auto BolickBot::ReceiveEvent(Mahjong::Event e) -> void
{
  if(e.decision)
  {
    lastEvent = e;
    
    
    
    
    
  }
  else if(e.type == Mahjong::Event::Discard && e.player == pid)
  {
    
    
    hand.push_back(e.piece);
    assignweights();
  }
  else if(e.type == Mahjong::Event::Discard)
  {
    discarded.push_back(e.piece);
  }
  else if(e.type == )
  {
      
  }
}

auto BolickBot::RetrieveDecision() -> Mahjong::Event
{
  if(lastEvent.type == Mahjong::Event::Discard)
  {
    
  }
  
  Mahjong::Event e = lastEvent;
  
  
  
  
  discarded.push_back(e.piece);
  lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
  return e;
}





