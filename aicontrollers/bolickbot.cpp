#include "bolickbot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind
#include <iostream>     // for endl, ostream, basic_ostream<>::__ostream_type
#include <memory>       // for allocator_traits<>::value_type

auto BolickBot::Name() -> std::string
{
  return "ThriceBot";
}

auto BolickBot::GameStart(int id) -> void 
{
  pid = id;
}

auto BolickBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind s, Mahjong::Wind p) -> void 
{
  for(Mahjong::Piece m : _hand)
  {
    handTile h;
    h.piece = m;
    if(m.isHonor())
    {
		h.weight = 1.5;
	}
	else
	{
		h.weight = 1;
    }
    hand.push_back(h);
  }
  assignweights();
  lastEvent.type = Mahjong::Event::Discard;
  swind = s;
  pwind = p;
}


void BolickBot::assignweights()
{
  for(int i =0; i < hand.size();i++)
  {
    for(int j = i; j < hand.size();j++)
    {
      if(j != i)
      {
        if((int)hand[i].weight == 2 && discardHas(hand[i].piece) == 2)
        {
          hand[i].weight = -1;
        }
        else if(discardHas(hand[i].piece) == 3)
        {
          hand[i].weight = -1;
        }
        else if(hand[i].piece.getSuit() == hand[j].piece.getSuit() && hand[i].piece.getPieceNum() == hand[j].piece.getPieceNum())
        {
          hand[i].weight += 1;
          hand[j].weight += 1;
        }
      }
    }
  }
}


int BolickBot::discardHas(Mahjong::Piece check)
{
  int has = 0;
  for(Mahjong::Piece p : discarded)
  {
    if(check.getSuit() == p.getSuit() && check.getPieceNum() == p.getPieceNum())
    {
      has++;
    }
  }
  return has;
}


auto BolickBot::ReceiveEvent(Mahjong::Event e) -> void
{
  if(e.decision)
  {
    if(e.type <= lastEvent.type)
    {
      lastEvent = e;
    }
  }
  if(e.type == Mahjong::Event::Discard && e.player == pid)
  {
    handTile h;
    h.piece = e.piece;
    if(e.piece.isHonor())
    {
		h.weight = 1.5;
	}
	else
	{
		h.weight = 1;
    }
    hand.push_back(h);
    assignweights();
  }
  else if(e.type == Mahjong::Event::Discard)
  {
    discarded.push_back(e.piece);
  }
}

auto BolickBot::RetrieveDecision() -> Mahjong::Event
{
  if(lastEvent.type == Mahjong::Event::Discard)
  {
    lastEvent.piece = popDiscard().toUint8_t();
  }
  Mahjong::Event e = lastEvent;
  lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
  return e;
}

Mahjong::Piece BolickBot::popDiscard()
{
  int indexOfLowest = 0;
  for (int i = 0; i < hand.size();i++)
  {
    if(hand[i].weight < hand[indexOfLowest].weight)
    {
      indexOfLowest = i;
    }
  }
  Mahjong::Piece p = hand[indexOfLowest].piece;
  hand.erase(hand.begin()+indexOfLowest);
  discarded.push_back(p);
  return p;
}



