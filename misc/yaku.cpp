#include "hand.h"
#include "pieces.h"

int AllSimples(const Hand& hand){
    int distance = 0;
    for(auto const& meld : hand.melds){
        for(auto const& piece: meld.pieces){
            if(isHonor(piece)){
                return -1;
            }
        }
    }
    for(auto const& piece : hand.pieces){
        if(isHonor(piece) || isTerminal(piece)){
            distance++;
        }
    }
    return distance;
}

void BreakdownHand(const Hand& hand){
    std::vector<std::vector<Meld>> possibleHands;
    for(auto const& piece: hand.pieces){
        
    }
}
