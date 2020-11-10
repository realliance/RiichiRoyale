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
    std::cout << "Started player " << id << std::endl;
}

auto AlphabeticalBot::RoundStart(std::vector<Mahjong::Piece> _hand, Mahjong::Wind, Mahjong::Wind) -> void {
    hand = _hand;
    decisionToTake.type = Mahjong::Event::Discard;
    decisionToTake.player = this->id;
}

auto AlphabeticalBot::ReceiveEvent(Mahjong::Event e) -> void{
    const Mahjong::Piece eventPiece = Mahjong::Piece(e.piece);
    //std::cout << "Player " << id <<" got event " << e << std::endl;
    if (e.type <= Mahjong::Event::Discard && e.decision && e.player == this->id)
    {
        if (e.type == Mahjong::Event::Discard)
        {
            if (e.type < decisionToTake.type)
            {
                //std::cout <<"Choosing "<< e.type << std::endl;
                decisionToTake = e;
            }
        }

    }
    switch (e.type)
    {
        case Mahjong::Event::Dora:
            break;
        case Mahjong::Event::Kan:
            if (e.decision)
            {
                decisionToTake.type = Mahjong::Event::Decline;
            }
            break;
        case Mahjong::Event::Chi:
            if (e.decision)
            {
                decisionToTake.type = Mahjong::Event::Decline;
            }
            break;
        case Mahjong::Event::Pon:
            if (e.decision)
            {
                decisionToTake.type = Mahjong::Event::Decline;
            }
            break;
        case Mahjong::Event::Discard:
            if (e.decision && e.player == this->id) {
                //std::cout << "Player " << this->id << " pushing piece into hand: " << e.piece << std::endl;
                hand.push_back(e.piece);
            }
            break;
    }
}

auto AlphabeticalBot::RetrieveDecision() -> Mahjong::Event{
    if (this->decisionToTake.type == Mahjong::Event::Discard)
    {
        auto indexToDiscard = getDiscardPiece();
        decisionToTake.piece = hand[indexToDiscard].raw_value();
        //std::cout << "Removing piece "<< indexToDiscard <<std::endl;
        hand.erase(hand.begin() + indexToDiscard);
    }
    auto final = this->decisionToTake;
    this->decisionToTake.type = Mahjong::Event::Discard;
    //std::cout << "Sending decision "<< final <<std::endl;
    return final;
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
    //std::cout << "getDiscardPiece(): " << index << std::endl;
    return index;
} 