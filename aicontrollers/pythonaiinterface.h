#pragma once
#include <event.h>      // for Event
#include <playercontroller.h>  // for MahjongAI
#include <winds.h>      // for Wind
#include <atomic>       // for atomic
#include <cstdint>      // for int16_t
#include <mutex>        // for mutex
#include <string>       // for string
#include <vector>       // for vector
#include <piecetype.h>

struct RoundStartStruct{
  std::vector<int16_t> hand;
  Mahjong::Wind seatWind;
  Mahjong::Wind prevalentWind;
};

class PythonAIInterface : public Mahjong::PlayerController{
public:
  PythonAIInterface();

  // Python Interface Functions
  auto PyGameStart() -> int; // Returns playerID
  auto PyRoundStart() -> RoundStartStruct;
  auto PyReceiveEvents() -> std::vector<Mahjong::Event>;
  auto PyRetrieveDecision(Mahjong::Event e) -> void;
  auto Name() -> std::string;

  // Engine Functions
  ~PythonAIInterface();
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void;
  auto ReceiveEvent(Mahjong::Event e) -> void;
  auto RetrieveDecision() -> Mahjong::Event;
  auto static Inst() -> PythonAIInterface*;
private:
  static PythonAIInterface* inst;
  std::mutex class_mutex;
  int playerID = -1;
  Mahjong::Event decision;
  std::vector<Mahjong::Event> events;
  RoundStartStruct roundStart;
  std::atomic<bool> playerIDRecieved = false;
  std::atomic<bool> decisionRecieved = false;
  std::atomic<bool> roundStartRecieved = false;
  std::atomic<bool> emptyEvents = true;
  static std::atomic<bool> instSet;
};
