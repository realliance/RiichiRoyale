#include <algorithm>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "walls.h"

PYBIND11_MODULE(RiichiRoyale, m) {
  py::class_<Walls>(m, "Walls")
    .def(py::init())
    .def("TakeHand", &Walls::TakeHand)
    .def("GetPath", &Walls::GetPath)
    .def("SetPath", &Walls::SetPath);
}
