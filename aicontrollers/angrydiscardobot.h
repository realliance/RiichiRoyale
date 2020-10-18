#pragma once
#include <string>       // for string
#include <vector>       // for vector
#include <event.h>      // for Event
#include <mahjongai.h>  // for MahjongAI
#include <winds.h>      // for Wind
class Piece;

//Always Calls (Angry) and Always Discards the tile it draws

class AngryDiscardoBot : public MahjongAI{
public:
  auto Name() -> std::string;
  auto GameStart(int _playerID) -> void;
  auto RoundStart(std::vector<Piece> board, Wind seatWind, Wind prevalentWind) -> void;
  auto ReceiveEvent(Event e) -> void;
  auto RetrieveDecision() -> Event;
private:
  Event lastEvent;
};
