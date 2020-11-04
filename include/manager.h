#pragma once
#include <string>
#include <vector>

namespace Mahjong{
  class PlayerController;
  using newControllerInst = PlayerController* (*)();

  auto GetAvailableControllers() -> std::vector<std::string>;
  auto GetController(std::string controller) -> newControllerInst;
  auto RegisterController(newControllerInst newFunc, std::string Name) -> bool;
  auto StartGame(std::vector<std::string> seatAIs, bool async) -> void;
};
