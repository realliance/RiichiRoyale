#include "thricebot.h"
#include "event.h"
#include "piecetype.h"
#include "winds.h"
#include <iostream>
#include <memory>


auto ThriceBot::Name() -> std::string
{
  return "ThriceBot";
}


auto ThriceBot::GameStart(int id) -> void 
{
  pid = id;
}


auto ThriceBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind s, Mahjong::Wind p) -> void 
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


void ThriceBot::assignweights()
{
  for(size_t i =0; i < hand.size();i++)
  {
    for(size_t j = i; j < hand.size();j++)
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


handTile ThriceBot::assignTileWeight(handTile h1)
{
  handTile h;
  h.piece = h1.piece;
  h.weight = h1.weight;
  for(size_t i =0; i < hand.size();i++)
  {
    if(hand[i].piece.getSuit() == h.piece.getSuit() && hand[i].piece.getPieceNum() == h.piece.getPieceNum())
    {
      hand[i].weight += 1;
      h.weight += 1;
    }
  }
  return h;
}


void ThriceBot::checkDiscard()
{
	for(size_t i =0; i < hand.size();i++)
  {
		for(size_t j = i; j < hand.size();j++)
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
		  }
		}
	}
}


int ThriceBot::discardHas(Mahjong::Piece check)
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


auto ThriceBot::ReceiveEvent(Mahjong::Event e) -> void
{
  if(e.decision)
  {
    if(e.type <= lastEvent.type)
    {
      lastEvent = e;
    }
  
    if(e.type == Mahjong::Event::Discard && e.player == pid)
    {
      handTile h;
      h.piece = e.piece;
      if(h.piece.isHonor())
      {
        h.weight = 1.5;
      }
      else
      {
        h.weight = 1;
      }
      h = assignTileWeight(h);
      hand.push_back(h);
      checkDiscard();
    }
    else if(e.type == Mahjong::Event::Discard)
    {
      discarded.push_back(e.piece);
    }
  }
}


auto ThriceBot::RetrieveDecision() -> Mahjong::Event
{
  if(lastEvent.type == Mahjong::Event::Discard)
  {
    lastEvent.piece = popDiscard().toUint8_t();
  }
  else if(lastEvent.type == Mahjong::Event::Pon)
  {
    Mahjong::Piece p = lastEvent.piece;
    if(!checkTile(p))
    {
      lastEvent.type = Mahjong::Event::Decline;
    }
  }
  else if(lastEvent.type == Mahjong::Event::Chi)
  {
    Mahjong::Piece p = lastEvent.piece;
    if(!checkTile(p))
    {
      lastEvent.type = Mahjong::Event::Decline;
    }
  }
  else if(lastEvent.type == Mahjong::Event::Riichi)
  {
    Mahjong::Piece p = lastEvent.piece;
    if(discardHas(p) == 3)
    {
      lastEvent.type = Mahjong::Event::Decline;
    }
  }
  Mahjong::Event e = lastEvent;
  lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
  return e;
}


bool ThriceBot::checkTile(Mahjong::Piece p)
{
  int j = 0;
  for(size_t i =0; i < hand.size();i++)
  {
    if(hand[i].piece.getSuit() == p.getSuit() && hand[i].piece.getPieceNum() == p.getPieceNum())
    {
      j++;
    }
  }
  if(j == 2 || j ==3)
  {
    return true;
  }
  return false;
}


Mahjong::Piece ThriceBot::popDiscard()
{
  int indexOfLowest = 0;
  for (size_t i = 0; i < hand.size();i++)
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
