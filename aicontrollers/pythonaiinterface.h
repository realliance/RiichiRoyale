#pragma once
#include <event.h>      // for Event
#include <mahjongai.h>  // for MahjongAI
#include <winds.h>      // for Wind
#include <atomic>       // for atomic
#include <cstdint>      // for int16_t
#include <mutex>        // for mutex
#include <string>       // for string
#include <vector>       // for vector
class Piece;

struct RoundStartStruct{
  std::vector<int16_t> hand;
  Wind seatWind;
  Wind prevalentWind;
};

class PythonAIInterface : public MahjongAI{
public:
  PythonAIInterface();

  // Python Interface Functions
  auto PyGameStart() -> int; // Returns playerID
  auto PyRoundStart() -> RoundStartStruct;
  auto PyReceiveEvents() -> std::vector<Event>;
  auto PyRetrieveDecision(Event e) -> void;
  auto Name() -> std::string;

  // Engine Functions
  ~PythonAIInterface();
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<Piece> hand, Wind seatWind, Wind prevalentWind) -> void;
  auto ReceiveEvent(Event e) -> void;
  auto RetrieveDecision() -> Event;
  auto static Inst() -> PythonAIInterface*;
private:
  static PythonAIInterface* inst;
  std::mutex class_mutex;
  int playerID = -1;
  Event decision;
  std::vector<Event> events;
  RoundStartStruct roundStart;
  std::atomic<bool> playerIDRecieved = false;
  std::atomic<bool> decisionRecieved = false;
  std::atomic<bool> roundStartRecieved = false;
  std::atomic<bool> emptyEvents = true;
  static std::atomic<bool> instSet;
};