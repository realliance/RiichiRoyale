#include <pybind11/pybind11.h>       // for class_, init, module
#include <pybind11/stl.h>

namespace py = pybind11;

#include "winds.h"
#include "manager.h"
#include "event.h"
#include "playercontroller.h"
#include "pythonaiinterface.h"

class PyMahjongAI: public Mahjong::PlayerController {
public:
  auto GameStart(int playerID) -> void{
    PYBIND11_OVERLOAD_PURE(
      void,
      Mahjong::PlayerController,
      GameStart,
      playerID
    );
  }
  auto RoundStart(std::vector<Mahjong::Piece> board, Mahjong::Wind seatWind, Mahjong::Wind prevalentWind) -> void{
    PYBIND11_OVERLOAD_PURE(
      void,
      Mahjong::PlayerController,
      RoundStart,
      board,
      seatWind,
      prevalentWind
    );
  }
  auto ReceiveEvent(Mahjong::Event e) -> void{
    PYBIND11_OVERLOAD_PURE(
      void,
      Mahjong::PlayerController,
      ReceiveEvent,
      e
    );
  }
  auto RetrieveDecision() -> Mahjong::Event{
    PYBIND11_OVERLOAD_PURE(
      Mahjong::Event,
      Mahjong::PlayerController,
      RetrieveDecision,
    );
  }
  auto Name() -> std::string{
    PYBIND11_OVERLOAD_PURE(
      std::string,
      Mahjong::PlayerController,
      Name,
    );
  }
};

PYBIND11_MODULE(libmahjong, m) {
  m.doc() = "Mahjong Game Driver";

  py::enum_<Mahjong::Wind>(m, "Wind")
    .value("East", Mahjong::Wind::East)
    .value("South", Mahjong::Wind::South)
    .value("West", Mahjong::Wind::West)
    .value("North", Mahjong::Wind::North)
    .export_values();

  py::enum_<Mahjong::Event::Type>(m, "EventType")
    .value("Ron", Mahjong::Event::Type::Ron)
    .value("Kan", Mahjong::Event::Type::Kan)
    .value("Pon", Mahjong::Event::Type::Pon)
    .value("Chi", Mahjong::Event::Type::Chi)
    .value("Tsumo", Mahjong::Event::Type::Tsumo)
    .value("ConcealedKan", Mahjong::Event::Type::ConcealedKan)
    .value("Riichi", Mahjong::Event::Type::Riichi)
    .value("Discard", Mahjong::Event::Type::Discard)
    .value("Decline", Mahjong::Event::Type::Decline)
    .value("ConvertedKan", Mahjong::Event::Type::ConvertedKan)
    .value("Dora", Mahjong::Event::Type::Dora)
    .value("PointDiff", Mahjong::Event::Type::PointDiff)
    .value("ExhaustiveDraw", Mahjong::Event::Type::ExhaustiveDraw)
    .value("End", Mahjong::Event::Type::End)
    .export_values();

  py::class_<Mahjong::Event>(m, "EngineEvent")
    .def(py::init<>())
    .def_readwrite("type", &Mahjong::Event::type)
    .def_readwrite("player", &Mahjong::Event::player)
    .def_readwrite("piece", &Mahjong::Event::piece)
    .def_readwrite("decision", &Mahjong::Event::decision);

  py::class_<Mahjong::PlayerController,PyMahjongAI>(m, "MahjongAI")
    .def(py::init<>())
    .def("GameStart", &Mahjong::PlayerController::GameStart)
    .def("RoundStart", &Mahjong::PlayerController::RoundStart)
    .def("ReceiveEvent", &Mahjong::PlayerController::ReceiveEvent)
    .def("RetrieveDecision", &Mahjong::PlayerController::RetrieveDecision)
    .def("Name", &Mahjong::PlayerController::Name);

  py::class_<PythonAIInterface>(m, "PythonAIInterface")
    .def(py::init<>())
    .def("GameStart", &PythonAIInterface::PyGameStart)
    .def("RoundStart", &PythonAIInterface::PyRoundStart)
    .def("ReceiveEvent", &PythonAIInterface::PyReceiveEvents)
    .def("RetrieveDecision", &PythonAIInterface::PyRetrieveDecision)
    .def("Name", &PythonAIInterface::Name)
    .def("Inst", &PythonAIInterface::Inst);

  py::class_<RoundStartStruct>(m, "RoundStartInfo")
    .def(py::init<>())
    .def_readonly("hand", &RoundStartStruct::hand)
    .def_readonly("seat_wind", &RoundStartStruct::seatWind)
    .def_readonly("prevalent_wind", &RoundStartStruct::prevalentWind);

  m.def("avaliable_ais", &Mahjong::GetAvailableControllers);
  m.def("register_ai", &Mahjong::RegisterController);
  m.def("start_game", &Mahjong::StartGame);
}
