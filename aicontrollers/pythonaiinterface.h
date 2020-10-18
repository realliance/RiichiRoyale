#pragma once
#include <event.h>      // for Event
#include <mahjongai.h>  // for MahjongAI
#include <piecetype.h>  // for Piece
#include <winds.h>      // for Wind
#include <atomic>       // for atomic
#include <mutex>        // for mutex
#include <string>       // for string
#include <vector>       // for vector

struct RoundStartStruct{
  std::vector<Piece> board;
  Wind seatWind;
  Wind prevalentWind;
};

class PythonAIInterface : public MahjongAI{
public:
  auto PyGameStart() -> int; // Returns playerID
  auto PyRoundStart() -> RoundStartStruct;
  auto PyReceiveEvents() -> std::vector<Event>;
  auto PyRetrieveDecision(Event e) -> void;
  auto Name() -> std::string;
  ~PythonAIInterface();
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<Piece> board, Wind seatWind, Wind prevalentWind) -> void;
  auto ReceiveEvent(Event e) -> void;
  auto RetrieveDecision() -> Event;
private:
  std::mutex class_mutex;
  int playerID = -1;
  Event decision;
  std::vector<Event> events;
  RoundStartStruct roundStart;
  std::atomic<bool> playerIDRecieved = false;
  std::atomic<bool> decisionRecieved = false;
  std::atomic<bool> roundStartRecieved = false;
  std::atomic<bool> emptyEvents = true;
};
