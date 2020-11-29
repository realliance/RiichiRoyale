#include <pybind11/detail/common.h>
#include <pybind11/detail/descr.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>            // IWYU pragma: keep
#include <cstdint>
#include <string>
#include <vector>
#include "event.h"
#include "controllermanager.h"
#include "statefunctions.h"
#include "piecetype.h"
#include "playercontroller.h"
#include "winds.h"
#include "settings.h"

namespace py = pybind11;

struct RoundStartStruct{
  std::vector<int16_t> hand;
  Mahjong::Wind seatWind;
  Mahjong::Wind prevalentWind;
};

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

  py::class_<Mahjong::Piece>(m, "Piece")
    .def(py::init<const uint8_t &>())
    .def("get_piece_num", &Mahjong::Piece::getPieceNum)
    .def("get_raw_value", &Mahjong::Piece::raw_value)
    .def("get_suit", &Mahjong::Piece::getSuit)
    .def("is_board_piece", &Mahjong::Piece::isBoardPiece)
    .def("is_red_five", &Mahjong::Piece::isRedFive)
    .def(py::self < py::self)
    .def(py::self == py::self)
    .def(py::self + int());

  py::enum_<Mahjong::Piece::Type>(m, "PieceType")
    .value("TERMINAL_BIT", Mahjong::Piece::Type::TERMINAL_BIT)
    .value("HONOR_SUIT", Mahjong::Piece::Type::HONOR_SUIT)
    .value("BAMBOO_SUIT", Mahjong::Piece::Type::BAMBOO_SUIT)
    .value("PIN_SUIT", Mahjong::Piece::Type::PIN_SUIT)
    .value("CHARACTER_SUIT", Mahjong::Piece::Type::CHARACTER_SUIT)
    .value("RED_FIVE", Mahjong::Piece::Type::RED_FIVE)
    .value("ERROR", Mahjong::Piece::Type::ERROR)
    .value("ONE_BAMBOO", Mahjong::Piece::Type::ONE_BAMBOO)
    .value("TWO_BAMBOO", Mahjong::Piece::Type::TWO_BAMBOO)
    .value("THREE_BAMBOO", Mahjong::Piece::Type::THREE_BAMBOO)
    .value("FOUR_BAMBOO", Mahjong::Piece::Type::FOUR_BAMBOO)
    .value("FIVE_BAMBOO", Mahjong::Piece::Type::FIVE_BAMBOO)
    .value("RED_FIVE_BAMBOO", Mahjong::Piece::Type::RED_FIVE_BAMBOO)
    .value("SIX_BAMBOO", Mahjong::Piece::Type::SIX_BAMBOO)
    .value("SEVEN_BAMBOO", Mahjong::Piece::Type::SEVEN_BAMBOO)
    .value("EIGHT_BAMBOO", Mahjong::Piece::Type::EIGHT_BAMBOO)
    .value("NINE_BAMBOO", Mahjong::Piece::Type::NINE_BAMBOO)
    .value("ONE_PIN", Mahjong::Piece::Type::ONE_PIN)
    .value("TWO_PIN", Mahjong::Piece::Type::TWO_PIN)
    .value("THREE_PIN", Mahjong::Piece::Type::THREE_PIN)
    .value("FOUR_PIN", Mahjong::Piece::Type::FOUR_PIN)
    .value("FIVE_PIN", Mahjong::Piece::Type::FIVE_PIN)
    .value("RED_FIVE_PIN", Mahjong::Piece::Type::RED_FIVE_PIN)
    .value("SIX_PIN", Mahjong::Piece::Type::SIX_PIN)
    .value("SEVEN_PIN", Mahjong::Piece::Type::SEVEN_PIN)
    .value("EIGHT_PIN", Mahjong::Piece::Type::EIGHT_PIN)
    .value("NINE_PIN", Mahjong::Piece::Type::NINE_PIN)
    .value("ONE_CHARACTER", Mahjong::Piece::Type::ONE_CHARACTER)
    .value("TWO_CHARACTER", Mahjong::Piece::Type::TWO_CHARACTER)
    .value("THREE_CHARACTER", Mahjong::Piece::Type::THREE_CHARACTER)
    .value("FOUR_CHARACTER", Mahjong::Piece::Type::FOUR_CHARACTER)
    .value("FIVE_CHARACTER", Mahjong::Piece::Type::FIVE_CHARACTER)
    .value("RED_FIVE_CHARACTER", Mahjong::Piece::Type::RED_FIVE_CHARACTER)
    .value("SIX_CHARACTER", Mahjong::Piece::Type::SIX_CHARACTER)
    .value("SEVEN_CHARACTER", Mahjong::Piece::Type::SEVEN_CHARACTER)
    .value("EIGHT_CHARACTER", Mahjong::Piece::Type::EIGHT_CHARACTER)
    .value("NINE_CHARACTER", Mahjong::Piece::Type::NINE_CHARACTER)
    .value("EAST_WIND", Mahjong::Piece::Type::EAST_WIND)
    .value("SOUTH_WIND", Mahjong::Piece::Type::SOUTH_WIND)
    .value("WEST_WIND", Mahjong::Piece::Type::WEST_WIND)
    .value("NORTH_WIND", Mahjong::Piece::Type::NORTH_WIND)
    .value("RED_DRAGON", Mahjong::Piece::Type::RED_DRAGON)
    .value("WHITE_DRAGON", Mahjong::Piece::Type::WHITE_DRAGON)
    .value("GREEN_DRAGON", Mahjong::Piece::Type::GREEN_DRAGON)
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

  py::class_<RoundStartStruct>(m, "RoundStartInfo")
    .def(py::init<>())
    .def_readonly("hand", &RoundStartStruct::hand)
    .def_readonly("seat_wind", &RoundStartStruct::seatWind)
    .def_readonly("prevalent_wind", &RoundStartStruct::prevalentWind);

  py::class_<Mahjong::GameSettings>(m, "GameSettings")
    .def(py::init<>())
    .def_readwrite("seat_controllers", &Mahjong::GameSettings::seatControllers)
    .def_readwrite("override_wall", &Mahjong::GameSettings::overrideWall)
    .def_readwrite("seed", &Mahjong::GameSettings::seed);


  m.def("avaliable_ais", &Mahjong::GetAvailableControllers);
  m.def("register_ai", &Mahjong::RegisterPythonController);
  m.def("unregister_ai", &Mahjong::UnregisterController);
  m.def("start_game", &Mahjong::StartGame);
  m.def("halt_game", &Mahjong::ExitGame);
}
