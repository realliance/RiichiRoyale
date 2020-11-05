#pragma once
#include <string>
#include <vector>
#include <functional>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace Mahjong{
  class PlayerController;
  using newControllerInst = std::function<PlayerController*()>;

  auto GetAvailableControllers() -> std::vector<std::string>;
  auto GetController(std::string controller) -> newControllerInst;
  auto RegisterController(newControllerInst newFunc, std::string Name) -> bool;
  auto RegisterPythonController(py::object pythonController, std::string Name) -> bool;
  auto StartGame(std::vector<std::string> seatAIs, bool async) -> void;
};
