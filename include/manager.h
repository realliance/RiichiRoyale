#pragma once
#include <string>
#include <vector>
#include <functional>

#ifndef NO_PYBIND
namespace pybind11 { class object; }
#endif

namespace Mahjong{
  class PlayerController;
  using newControllerInst = std::function<PlayerController*()>;

  auto GetAvailableControllers() -> std::vector<std::string>;
  auto GetController(std::string controller) -> newControllerInst;
  auto RegisterController(newControllerInst newFunc, std::string Name) -> bool;
#ifndef NO_PYBIND
  auto RegisterPythonController(pybind11::object pythonController, std::string Name) -> bool;
#endif
  auto StartGame(std::vector<std::string> seatAIs, bool async) -> void;
};
