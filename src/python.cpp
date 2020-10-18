#include <pybind11/pybind11.h>       // for class_, init, module
#include <pybind11/stl.h>

namespace py = pybind11;

#include "winds.h"
#include "player.h"
#include "manager.h"
#include "event.h"
#include "mahjongai.h"
#include "pythonaiinterface.h"

class PyMahjongAI: public MahjongAI {
public:
  auto GameStart(int playerID) -> void{
    PYBIND11_OVERLOAD_PURE(
      void,
      MahjongAI,
      GameStart,
      playerID
    );
  }
  auto RoundStart(std::vector<Piece> board, Wind seatWind, Wind prevalentWind) -> void{
    PYBIND11_OVERLOAD_PURE(
      void,
      MahjongAI,
      RoundStart,
      board,
      seatWind,
      prevalentWind
    );
  }
  auto ReceiveEvent(Event e) -> void{
    PYBIND11_OVERLOAD_PURE(
      void,
      MahjongAI,
      ReceiveEvent,
      e
    );
  }
  auto RetrieveDecision() -> Event{
    PYBIND11_OVERLOAD_PURE(
      Event,
      MahjongAI,
      RetrieveDecision,
    );
  }
  auto Name() -> std::string{
    PYBIND11_OVERLOAD_PURE(
      std::string,
      MahjongAI,
      Name,
    );
  }
};

PYBIND11_MODULE(libmahjong, m) {
  m.doc() = "Mahjong Game Driver";

  py::class_<Player>(m, "Player")
    .def(py::init<>())
    .def_readonly("points", &Player::points)
    .def_readonly("position", &Player::position)
    .def_readonly("seat", &Player::seat);

  py::enum_<Wind>(m, "Wind")
    .value("East", Wind::East)
    .value("South", Wind::South)
    .value("West", Wind::West)
    .value("North", Wind::North)
    .export_values();

  py::enum_<EventType>(m, "EventType")
    .value("Ron", EventType::Ron)
    .value("Kan", EventType::Kan)
    .value("Pon", EventType::Pon)
    .value("Chi", EventType::Chi)
    .value("Tsumo", EventType::Tsumo)
    .value("ConcealedKan", EventType::ConcealedKan)
    .value("Riichi", EventType::Riichi)
    .value("Discard", EventType::Discard)
    .value("Decline", EventType::Decline)
    .value("Dora", EventType::Dora)
    .value("PointDiff", EventType::PointDiff)
    .value("ExhaustiveDraw", EventType::ExhaustiveDraw)
    .value("End", EventType::End)
    .export_values();

  py::class_<Event>(m, "Event")
    .def(py::init<>())
    .def_readonly("type", &Event::type)
    .def_readonly("player", &Event::player)
    .def_readonly("piece", &Event::piece)
    .def_readonly("decision", &Event::decision);

  py::class_<MahjongAI,PyMahjongAI>(m, "MahjongAI")
    .def(py::init<>())
    .def("GameStart", &MahjongAI::GameStart)
    .def("RoundStart", &MahjongAI::RoundStart)
    .def("ReceiveEvent", &MahjongAI::ReceiveEvent)
    .def("RetrieveDecision", &MahjongAI::RetrieveDecision)
    .def("Name", &MahjongAI::Name);

  py::class_<PythonAIInterface>(m, "PythonAIInterface")
    .def(py::init<>())
    .def("GameStart", &PythonAIInterface::PyGameStart)
    .def("RoundStart", &PythonAIInterface::PyRoundStart)
    .def("ReceiveEvent", &PythonAIInterface::PyReceiveEvents)
    .def("RetrieveDecision", &PythonAIInterface::PyRetrieveDecision)
    .def("Name", &PythonAIInterface::Name)
    .def("Inst", &PythonAIInterface::Inst);

  py::class_<MahjongGameManager>(m, "MahjongGameManager")
    .def("avaliable_ais", &MahjongGameManager::GetAvailableAIs)
    .def("register_ai", &MahjongGameManager::RegisterAI)
    .def("start_game", &MahjongGameManager::StartGame);
}
