#pragma once
#include <string>
#include <vector>
#include <event.h>
#include <playercontroller.h>
#include <piecetype.h>
#include <winds.h>

//Always Calls (Angry) and Always Discards the tile it draws

class AlphabeticalBot : public Mahjong::PlayerController{
public:
    auto Name() -> std::string;
    auto GameStart(int _playerID) -> void;
    auto RoundStart(std::vector<Mahjong::Piece> hand, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void;
    auto ReceiveEvent(Mahjong::Event e) -> void;
    auto RetrieveDecision() -> Mahjong::Event;
private:
    std::vector<Mahjong::Piece> hand;
    int n = 0;
    int id;
    Mahjong::Event lastEvent;
    int getDiscardPiece();
};