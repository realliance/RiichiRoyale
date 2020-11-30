#include "gentlemanbot.h"
#include "event.h"
#include "piecetype.h"
#include "winds.h"
#include "analysis.h"
#include "handnode.h"
#include <iostream>
#include <memory>

auto GentlemanBot::Name() -> std::string{
  return "GentlemanBot";
}

auto GentlemanBot::GameStart(int) -> void {}

auto GentlemanBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind, Mahjong::Wind) -> void {
  hand = _hand;
  lastEvent.type = Mahjong::Event::Discard;
  riichi=false;

}

auto GentlemanBot::ReceiveEvent(Mahjong::Event e) -> void{

  if(e.decision){
    if(e.type <= lastEvent.type)
    lastEvent = e;

    if(e.type == Mahjong::Event::Discard){
      if(!riichi){
        hand.push_back(e.piece);
        lastEvent.piece = getDiscard().toUint8_t();
      }
    }
    else{
    }
  }
  
  
}

auto GentlemanBot::RetrieveDecision() -> Mahjong::Event{
  if (lastEvent.type == Mahjong::Event::Riichi){
    lastEvent.type = Mahjong::Event::Riichi;
    riichi=true;
  }
  else if(lastEvent.type != Mahjong::Event::Discard){
    lastEvent.type = Mahjong::Event::Decline;
  }


  Mahjong::Event e = lastEvent;
  lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
  return e;
}


void countpieces(int8_t* counts, const std::vector<Mahjong::Piece>& pieces){
  for(const auto & p : pieces){
    counts[p.toUint8_t()]++;
  }
}


auto GentlemanBot::getDiscard() -> Mahjong::Piece{

  std::vector<Mahjong::Piece> freePieces;
  std::vector<Mahjong::Piece> preferedDiscards;
  std::vector<Mahjong::Piece> secondTierDiscards;
  std::vector<Mahjong::Piece> thirdTierDiscards;
  
  int8_t counts[256] = {};
  Mahjong::Node* symbolicHand = Mahjong::breakdownHand(hand);
  Mahjong::Node* currentNode = symbolicHand;

  while(true) {
    if(currentNode->type == Mahjong::Node::Single){
      freePieces.push_back(currentNode->start);
    }
    if(currentNode->leaves.size() == 0){
      break;
    }
    
    currentNode = currentNode->leaves[0];
  }


  delete symbolicHand;
  
  countpieces(counts, freePieces);

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
      preferedDiscards.push_back(p);
    }
  }

  for(const auto &p : preferedDiscards){
    if(p.isHonor()){
      for(size_t i=0; i<hand.size(); i++){
        if(hand[i].toUint8_t() == p.toUint8_t()){
          hand.erase(hand.begin()+i);
        }
      }
      return p;
    }
  }
  for(const auto &p : preferedDiscards){
    for(size_t i=0; i<hand.size(); i++){
        if(hand[i].toUint8_t() == p.toUint8_t()){
          hand.erase(hand.begin()+i);
        }
      }
      return p;
  }
  for(const auto &p : secondTierDiscards){
    for(size_t i=0; i<hand.size(); i++){
        if(hand[i].toUint8_t() == p.toUint8_t()){
          hand.erase(hand.begin()+i);
        }
      }
      return p;
  }
  for(const auto &p : thirdTierDiscards){
    for(size_t i=0; i<hand.size(); i++){
        if(hand[i].toUint8_t() == p.toUint8_t()){
          hand.erase(hand.begin()+i);
        }
      }
      return p;
  }
  return Mahjong::Piece::ERROR;
}
