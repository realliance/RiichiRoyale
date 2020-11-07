#include "alphabeticalbot.h"
#include "event.h"      // for Event, Decline
#include "piecetype.h"  // for Piece
#include "winds.h"      // for Wind
#include <iostream>     // for endl, ostream, basic_ostream<>::__ostream_type
#include <memory>       // for allocator_traits<>::value_type

auto AlphabeticalBot::Name() -> std::string{
    return "AlphabeticalBot";
}

auto AlphabeticalBot::GameStart(int id) -> void{
    this->id = id;
}

auto AlphabeticalBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind, Mahjong::Wind) -> void {
    hand = _hand;
    lastEvent.type = Mahjong::Event::Discard;
}

auto AlphabeticalBot::ReceiveEvent(Mahjong::Event e) -> void{
    if(e.decision){
        if(e.type <= lastEvent.type)
            lastEvent = e;
    }else if(e.type == Mahjong::Event::Discard){
        hand.push_back(e.piece);
    }
}

auto AlphabeticalBot::RetrieveDecision() -> Mahjong::Event{
    if (lastEvent.player == this->id && lastEvent.decision) //event needs decision
    {
        printf("Needs decision from player %d on event %d.\n",this->id, lastEvent.type);
        if (lastEvent.type == Mahjong::Event::Discard){
            int indexToDiscard = getDiscardPiece();
            lastEvent.piece = hand[indexToDiscard].toUint8_t();
            hand.erase(hand.begin() + indexToDiscard);
            printf("%d discarded piece %d (index %d), hand size is now: %d\n", this->id, lastEvent.piece, indexToDiscard,  hand.size());
            Mahjong::Event e = lastEvent;
            e.type = Mahjong::Event::Discard;
            e.decision = false;
            return e;
        }
        else if (lastEvent.type < Mahjong::Event::Decline)
        {
            Mahjong::Event e = lastEvent;
            e.type = Mahjong::Event::Decline; //skip all calls
            return e;
        }
        else
        {
            printf("Got unimplemented event.\n");
        }
    }
    else
    {
        printf("Shouldn't get here\n");
    }
    Mahjong::Event e = lastEvent;
    lastEvent.type = Mahjong::Event::Discard; // lowest """priority""" event type
    return e;
}


//Choose a piece to discard (
auto AlphabeticalBot::getDiscardPiece() -> int{

    std::vector<std::string> stringBoard;
    for (long unsigned int i = 0; i<this->hand.size(); i++)
    {
        stringBoard.push_back(this->hand[i].toStr());
    }

    int index = 0;
    for (long unsigned int i = 1; i < stringBoard.size(); i++) //find index of first string (alphabetically)
    {
        if (stringBoard[i]<stringBoard[index])
        {
            index = i;
        }
    }
    return index;
} 