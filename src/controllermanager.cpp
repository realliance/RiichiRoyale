#include "controllermanager.h"
#include <angrydiscardobot.h>
#include <thricebot.h>
#include <fasttanyao.h>
#include <alphabeticalbot.h>
#include <totobot.h>
#include <gentlemanbot.h>
#include <map>
#include <string>
#include <vector>
#include "statefunctions.h"
#include "playercontroller.h"
#include "settings.h"
#ifndef NO_PYBIND
#include <pybind11/stl.h>      // IWYU pragma: keep
#include <pybind11/cast.h>
#include <pybind11/pytypes.h>
#endif

using namespace Mahjong;

static std::map<std::string,newControllerInst> availableControllers = {
  {"AngryDiscardoBot",[]() -> PlayerController* {return new AngryDiscardoBot;}},
  {"Fast Tanyao",[]() -> PlayerController* {return new FastTanyao;}},
  {"TotoBot",[]() -> PlayerController* {return new TotoBot;}},
  {"GentlemanBot",[]() -> PlayerController* {return new GentlemanBot;}},
  {"AlphabeticalBot",[]() -> PlayerController* {return new AlphabeticalBot;}},
  {"ThriceBot",[]() -> PlayerController* {return new ThriceBot;}},
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

auto Mahjong::UnregisterController(std::string Name) -> void {
  availableControllers.erase(Name);
}
#endif
