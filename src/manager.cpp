#include "manager.h"
#include <angrydiscardobot.h>  // for AngryDiscardoBot
#include <bolickbot.h>         // for ThriceBot
#include <fasttanyao.h>
#include <alphabeticalbot.h>   // for AlphabeticalBot
#include <totobot.h>
#include <gentlemanbot.h>
#include <map>                 // for map, map<>::mapped_type
#include <string>              // for string, operator+
#include <thread>              // for thread
#include <vector>              // for vector
#include "statefunctions.h"    // for StateController
#include "playercontroller.h"
#include "settings.h"          // for GameSettings
#ifndef NO_PYBIND
#include <pybind11/stl.h>      // IWYU pragma: keep
#include <pybind11/cast.h>     // for object::cast
#include <pybind11/pytypes.h>  // for object
#endif

using namespace Mahjong;

auto Mahjong::StartGame(GameSettings settings, bool async) -> void {
  if(async){
    std::thread gameloop(&Mahjong::StateController, settings);
    gameloop.detach();
  }else{
    StateController(settings);
  }
}

static std::map<std::string,newControllerInst> availableControllers = {
  {"AngryDiscardoBot",[]() -> PlayerController* {return new AngryDiscardoBot;}},
  {"Fast Tanyao",[]() -> PlayerController* {return new FastTanyao;}},
  {"TotoBot",[]() -> PlayerController* {return new TotoBot;}},
  {"GentlemanBot",[]() -> PlayerController* {return new GentlemanBot;}},
  {"AlphabeticalBot",[]() -> PlayerController* {return new AlphabeticalBot;}},
  {"ThriceBot",[]() -> PlayerController* {return new BolickBot;}},
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


#ifndef NO_PYBIND
auto Mahjong::RegisterPythonController(pybind11::object pythonController, std::string Name) -> bool {
  auto genFunction = [=]() -> PlayerController* {
    return pythonController.cast<PlayerController*>(); 
  };
  return Mahjong::RegisterController(genFunction, Name);
}
#endif
