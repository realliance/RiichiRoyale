#include "gentlemanbot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind
#include "analysis.h"   // for handtree?
#include "handnode.h"
#include <iostream>     // for endl, ostream, basic_ostream<>::__ostream_type
#include <memory>       // for allocator_traits<>::value_type

auto GentlemanBot::Name() -> std::string{
  return "GentlemanBot";
}

auto GentlemanBot::GameStart(int) -> void {}

auto GentlemanBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind, Mahjong::Wind) -> void {
  hand = _hand;
  lastEvent.type = Mahjong::Event::Discard;
}

auto GentlemanBot::ReceiveEvent(Mahjong::Event e) -> void{

  if(e.decision){
    lastEvent = e;
  }
  else if(e.type == Mahjong::Event::Discard){
    hand.push_back(e.piece);
    

  }
}

auto GentlemanBot::RetrieveDecision() -> Mahjong::Event{
  if(lastEvent.type == Mahjong::Event::Ron){
    lastEvent.type = Mahjong::Event::Ron;
  }
  else if(lastEvent.type == Mahjong::Event::Tsumo){
    lastEvent.type = Mahjong::Event::Tsumo;
  }
  else if (lastEvent.type == Mahjong::Event::Riichi){
    lastEvent.type = Mahjong::Event::Riichi;
  }
  else if (lastEvent.type == Mahjong::Event::ConcealedKan){
    lastEvent.type = Mahjong::Event::ConcealedKan;
  }
  else if(lastEvent.type == Mahjong::Event::Discard){
    lastEvent.piece = getDiscard();
  }
  else{
    lastEvent.type = Mahjong::Event::Decline;
  }
  Mahjong::Event e = lastEvent;
  lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
  return e;
}


void countPieces(int8_t* counts, const std::vector<Piece>& pieces){
  for(const auto & p : pieces){
    counts[p.toUint8_t()]++;
  }
}


auto getDiscard() -> Mahjong::Piece{

  std::vector<Piece> freePieces;
  std::vector<Piece> preferedDiscards;
  std::vector<Piece> secondTierDiscards;
  std::vector<Piece> thirdTierDiscards;
  
  int8_t counts[256] = {};
  Node* symbolicHand = breakdownHand(hand);

  if(symbolicHand->type == Node::Single){
    freePieces.push(symbolicHand->start)
  }

  delete symbolicHand;
  
  countPieces(counts, freePieces);

  for(const auto & p : freePieces){
    if(counts[p.toUint8_t()]==2){
      thirdTierDiscards.push_back(p);
    }
    else if(counts[(p+1).toUint8_t()]>0 || counts[(p-1).toUint8_t()]>0) {
      secondTierDiscards.push_back(p);
    }
    else if (counts[(p+2).toUint8_t()]>0 || counts[(p-2).toUint8_t()]>0){
      thirdTierDiscards.push_back(p);
    }
    else{
      preferedDiscards.pushback(p);
    }
  }

  for(const auto & p : preferedDiscards){
    if(p.isHonor){
      return p;
    }
  }
  for(const auto & p : preferedDiscards){
      return p;
  }
  for(const auto & p : secondTierDiscards){
      return p;
  }
  for(const auto & p : thirdTierDiscards){
      return p;
  }

}