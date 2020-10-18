#include "pythonaiinterface.h"
#include <event.h>      // for Event, END_EVENT
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind

PythonAIInterface* PythonAIInterface::inst;

PythonAIInterface::PythonAIInterface(){
  inst = this;
}

PythonAIInterface::~PythonAIInterface(){
  while(!emptyEvents){}
  const std::lock_guard<std::mutex> lock(class_mutex);
}

auto PythonAIInterface::GameStart(int _playerID) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  playerID = _playerID;
  playerIDRecieved = true;
}

auto PythonAIInterface::RoundStart(std::vector<Piece> board, Wind seatWind, Wind prevalentWind) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  roundStart.board = board;
  roundStart.seatWind = seatWind;
  roundStart.prevalentWind = prevalentWind;
  roundStartRecieved = true;
}

auto PythonAIInterface::ReceiveEvent(Event e) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  if(e.type == PointDiff){
    playerIDRecieved = false;
    roundStartRecieved = false;
    decisionRecieved = false;
  }
  emptyEvents = false;
  events.push_back(e);
}

auto PythonAIInterface::RetrieveDecision() -> Event{
  while(!decisionRecieved){}
  const std::lock_guard<std::mutex> lock(class_mutex);
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
  emptyEvents = false;
  return eventCopy;
}

auto PythonAIInterface::PyRetrieveDecision(Event e) -> void{
  const std::lock_guard<std::mutex> lock(class_mutex);
  decision = e;
  decisionRecieved = true;
}
