#pragma once
#include <string>
#include <vector>
#include <set>
#include <functional>

#ifndef NO_PYBIND
namespace pybind11 { class object; }
#endif

namespace Mahjong { struct GameSettings; class PlayerController; }

namespace Mahjong{
  using newControllerInst = std::function<PlayerController*()>;

  auto GetAvailableControllers() -> std::vector<std::string>;
  auto GetController(std::string controller) -> newControllerInst;
  auto RegisterController(newControllerInst newFunc, std::string Name) -> bool;
#ifndef NO_PYBIND
  static std::set<std::string> pythonManagedControllers;
  auto RegisterPythonController(pybind11::object pythonController, std::string Name) -> bool;
  auto UnregisterController(std::string Name) -> void;
#endif
};
