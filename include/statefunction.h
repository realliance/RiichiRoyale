#pragma once
#include "roundstate.h"

struct stateFunction{
private:
    stateFunction (*func)(RoundState&);
public:
  stateFunction(stateFunction (*func)(RoundState&) ):func(func){}
  stateFunction operator()(RoundState& state){ return func(state); }
};
