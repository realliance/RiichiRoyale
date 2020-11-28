#pragma once
#include <string>
#include <vector>
#include <event.h>
#include <playercontroller.h>
#include <piecetype.h>
#include <winds.h>

struct handTile
{
  Mahjong::Piece piece;
  float weight;
};

class ThriceBot : public Mahjong::PlayerController
{
	public:
	  auto Name() -> std::string;
	  auto GameStart(int _playerID) -> void;
	  auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void;
	  auto ReceiveEvent(Mahjong::Event e) -> void;
	  auto RetrieveDecision() -> Mahjong::Event;
	private:
	  //private functions
	  void assignweights(void);
	  int discardHas(Mahjong::Piece);
	  Mahjong::Piece popDiscard(void);
	  //instance vars
	  std::vector<handTile> hand;
	  std::vector<Mahjong::Piece> discarded;
	  Mahjong::Event lastEvent;
	  Mahjong::Wind swind;
	  Mahjong::Wind pwind;
      int pid;
};
