#pragma once
#include <vector>
#include <array>
#include <string>

#include "event.h"
#include "piecetype.h"

class MahjongGameManager;
class MahjongAI{
public:
  virtual ~MahjongAI(){}
  virtual auto GameStart(int playerID) -> void = 0;
  virtual auto RoundStart(std::vector<Piece> hand, Wind seatWind, Wind prevalentWind) -> void = 0;
  virtual auto ReceiveEvent(Event e) -> void = 0;
  virtual auto RetrieveDecision() -> Event = 0;
  virtual auto Name() -> std::string = 0;
};
