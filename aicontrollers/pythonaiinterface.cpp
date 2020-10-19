#include "pythonaiinterface.h"
#include <event.h>      // for Event, operator<<, PointDiff
#include <iostream>     // for endl, operator<<, ostream, cout, basic_ostream
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind

PythonAIInterface* PythonAIInterface::inst;
std::atomic<bool> PythonAIInterface::instSet = false;

PythonAIInterface::PythonAIInterface(){
  instSet = true;
  inst = this;
}

auto PythonAIInterface::Inst() -> PythonAIInterface*{
  while(!instSet){}
  std::cout << "==================" << inst << std::endl;
  return inst;
}

PythonAIInterface::~PythonAIInterface(){
  while(!emptyEvents){}
  const std::lock_guard<std::mutex> lock(class_mutex);
  instSet = false;
  inst = nullptr;
}

auto PythonAIInterface::GameStart(int _playerID) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  playerID = _playerID;
  playerIDRecieved = true;
}

auto PythonAIInterface::RoundStart(std::vector<Piece> hand, Wind seatWind, Wind prevalentWind) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  for(const auto & piece : hand){
    roundStart.hand.push_back(static_cast<int16_t>(piece.toUint8_t()));
  }
  roundStart.seatWind = seatWind;
  roundStart.prevalentWind = prevalentWind;
  roundStartRecieved = true;
}

auto PythonAIInterface::ReceiveEvent(Event e) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  if(e.type == PointDiff){
    roundStartRecieved = false;
    decisionRecieved = false;
  }
  emptyEvents = false;
  events.push_back(e);
}

auto PythonAIInterface::RetrieveDecision() -> Event{
  while(!decisionRecieved){}
  const std::lock_guard<std::mutex> lock(class_mutex);
  std::cout << decision << std::endl;
  decisionRecieved = false;
  return decision;
}

auto PythonAIInterface::Name() -> std::string{
  return "PythonAIInterface";
}


auto PythonAIInterface::PyGameStart() -> int{
  while(!playerIDRecieved){}
  const std::lock_guard<std::mutex> lock(class_mutex);
  return playerID;
}

auto PythonAIInterface::PyRoundStart() -> RoundStartStruct{
  while(!roundStartRecieved){}
  const std::lock_guard<std::mutex> lock(class_mutex);
  return roundStart;
}

auto PythonAIInterface::PyReceiveEvents() -> std::vector<Event>{
  const std::lock_guard<std::mutex> lock(class_mutex);
  std::vector<Event> eventCopy;
  std::swap(eventCopy,events);
  emptyEvents = true;
  return eventCopy;
}

auto PythonAIInterface::PyRetrieveDecision(Event e) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  decision = e;
  decisionRecieved = true;
}
