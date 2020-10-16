#pragma once
#include "gamestate.h"

struct stateFunction{
private:
    stateFunction (*func)(GameState&);
public:
  stateFunction(stateFunction (*func)(GameState&) ):func(func){}
  stateFunction operator()(GameState& state){ return func(state); }
};
