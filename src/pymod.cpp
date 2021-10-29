#include "controller.h"
#include "pybind11/detail/common.h"
#include "pybind11/pybind11.h"

#include "ptzf_config.h"
#include "position.h"
#include "controller.h"
#include "utils.h"

namespace py = pybind11;

// clang-format off

PYBIND11_MODULE(pyptzf, m) {
  m.doc() = ptzf::DESCRIPTION;
  m.attr("__version__") = ptzf::VERSION;

  py::class_<ptzf::Position>(m, "Position")
    .def(py::init<>())
    .def_readwrite("x", &ptzf::Position::x)
    .def_readwrite("y", &ptzf::Position::y)
    .def_readwrite("z", &ptzf::Position::z)
    .def_readwrite("f", &ptzf::Position::f)
    .def("is_valid", &ptzf::Position::is_valid)
    .def_static("max", &ptzf::Position::max)
    .def_static("min", &ptzf::Position::min)
    .def("__str__", ptzf::position_to_string);

  py::class_<ptzf::Controller>(m, "Controller")
    // The gil must be released any IO to not hang python when using the
    // threading module. This way IO can be overlapped with Python work.
    // https://pybind11.readthedocs.io/en/stable/advanced/misc.html#global-interpreter-lock-gil
    .def(py::init<std::string>(), py::call_guard<py::gil_scoped_release>())
    .def(py::init<std::string, bool>(), py::call_guard<py::gil_scoped_release>())
    .def("connect", &ptzf::Controller::connect)
    .def("disconnect", &ptzf::Controller::disconnect)
    .def("is_connected", &ptzf::Controller::is_connected)
    .def("go", &ptzf::Controller::go, py::call_guard<py::gil_scoped_release>());
}

// clang-format on