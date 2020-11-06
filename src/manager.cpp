#include "manager.h"

#include <stddef.h>            // for size_t
#include <algorithm>           // for find
#include <array>               // for array
#include <cstdint>             // for int16_t
#include <iostream>            // for operator<<, endl, cerr, ostream, basic...
#include <map>                 // for map
#include <memory>              // for allocator_traits<>::value_type
#include <string>              // for string
#include <vector>              // for vector
#include <thread>
#include <functional>
#include <angrydiscardobot.h>
#include <pythonaiinterface.h>
#include <pybind11/pybind11.h>

#include "event.h"             // for Event, Kan, Discard, Chi, ConcealedKan
#include "gamestate.h"         // for GameState, AfterCall, AfterDraw, opera...
#include "hand.h"              // for Hand
#include "hands.h"             // for isComplete, isInAValidFormat, Tenpai
#include "meld.h"              // for Meld, PonMeld
#include "piecetype.h"         // for Piece, ERROR_PIECE
#include "player.h"            // for Player
#include "walls.h"             // for Walls
             // for Wind, South
#include "statefunctions.h"

using namespace Mahjong;
namespace py = pybind11;

auto Mahjong::StartGame(std::vector<std::string> playerAIs, bool async) -> void {
  if(async){
    std::thread gameloop(&Mahjong::StateController, playerAIs);
    gameloop.detach();
  }else{
    StateController(playerAIs);
  }
}

static std::map<std::string,newControllerInst> availableControllers = {
  {"AngryDiscardoBot",[]() -> PlayerController* {return new AngryDiscardoBot;}},
};

auto Mahjong::GetAvailableControllers() -> std::vector<std::string> {
  std::vector<std::string> names;
  for(const auto &[name, _] : availableControllers){
    names.push_back(name);
  }
  return names;
}

auto Mahjong::GetController(std::string controller) -> newControllerInst{
  if(availableControllers.contains(controller)){
    return availableControllers[controller];
  }
  throw "No Such Controller";
}

auto Mahjong::RegisterController(newControllerInst newFunc, std::string name) -> bool {
  if(availableControllers.contains(name)){
    return false;
  }
  availableControllers[name] = newFunc;
  return true;
}


auto Mahjong::RegisterPythonController(py::object pythonController, std::string Name) -> bool {
  auto genFunction = [=]() -> PlayerController* {
    return pythonController.cast<PlayerController*>(); 
  };
  return Mahjong::RegisterController(genFunction, Name);
}