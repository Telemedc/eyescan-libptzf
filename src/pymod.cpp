#include "pybind11/pybind11.h"
#include "pybind11/detail/common.h"
#include <pybind11/operators.h>
#include "pybind11/cast.h"
#include "pybind11/attr.h"

#include <exception>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

#include "ptzf_config.h"
#include "controller.h"
#include "position.h"
#include "controller.h"
#include "pybind11/pytypes.h"
#include "utils.h"

namespace py = pybind11;


// clang-format off

PYBIND11_MODULE(pyptzf, m) {
  m.doc() = ptzf::DESCRIPTION;
  m.attr("__version__") = ptzf::VERSION;

  static py::exception<nlohmann::detail::exception> exc(m, "NlohmannJsonError");
  py::register_exception_translator([](std::exception_ptr p) {
    try {
      if (p) std::rethrow_exception(p);
    } catch (const nlohmann::detail::exception &e) {
      exc(e.what());
    }
  });

  // ptzf::Position class
  py::class_<ptzf::Position> pyptzf_position(m, "Position");

  // pyptzf.Position.Key enum
  py::enum_<ptzf::Position::Key>(pyptzf_position, "Key")
    .value("x", ptzf::Position::Key::x)
    .value("y", ptzf::Position::Key::y)
    .value("z", ptzf::Position::Key::z)
    .value("f", ptzf::Position::Key::f);

#pragma GCC diagnostic push
// because GCC doesn't recognize the `self-assign-overloaded` below
#pragma GCC diagnostic ignored "-Wpragmas"
// for operator -=
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"
  pyptzf_position.def(
      py::init<double, double, double, double>(),
      py::arg("x") = 0.0,
      py::arg("y") = 0.0,
      py::arg("z") = 0.0,
      py::arg("f") = 0.0)
    .def_readwrite("x", &ptzf::Position::x)
    .def_readwrite("y", &ptzf::Position::y)
    .def_readwrite("z", &ptzf::Position::z)
    .def_readwrite("f", &ptzf::Position::f)
    .def_static("from_json", ptzf::Position::from_json)
    .def("to_json", &ptzf::Position::to_json)
    .def_static("keys", [](){
      return py::make_tuple("x", "y", "z", "f");
    })
    .def(py::self + py::self)
    .def(py::self - py::self)
    .def(py::self += py::self)
    .def(py::self -= py::self)
    .def(py::self * py::self)
    .def(py::self *= py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self > py::self)
    .def(py::self >= py::self)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def("__getitem__", &ptzf::Position::operator[])
    .def("__getitem__", [](const ptzf::Position& p, std::string key){
      if (key == "x") {
        return p.x;
      } else if (key == "y") {
        return p.y;
      } else if (key == "z") {
        return p.z;
      } else if (key == "f") {
        return p.f;
      } else {
        std::stringstream ss;
        ss << "`" << key << "`" << " not a valid key (use `x`,`y`,`z` or `f`)";
        throw py::key_error(ss.str());
      }
    })
    .def("__setitem__", [](ptzf::Position& p, ptzf::Position::Key key, float v){
      switch (key) {
        case (ptzf::Position::Key::x): {
          p.x = v;
          break;
        }
        case (ptzf::Position::Key::y): {
          p.y = v;
          break;
        }
        case (ptzf::Position::Key::z): {
          p.z = v;
          break;
        }
        case (ptzf::Position::Key::f): {
          p.f = v;
          break;
        }
        default: {
          throw py::key_error("Invalid key. Probably __setitem__ broken.");
        }
      }
    })
    .def("__setitem__", [](ptzf::Position& p, std::string key, float v){
      if (key == "x") {
        p.x = v;
      } else if (key == "y") {
        p.y = v;
      } else if (key == "z") {
        p.z = v;
      } else if (key == "f") {
        p.f = v;
      } else {
        std::stringstream ss;
        ss << "`" << key << "`" << " not a valid key (use `x`,`y`,`z` or `f`)";
        throw py::key_error(ss.str());
      }
    })
    .def_static("__contains__", [](std::string key) {
      return key == "x" || key == "y" || key == "z" || key == "f";
    }) 
    .def_static("__contains__", [](ptzf::Position::Key k){
      (void)k;  // if a `Key` is used, it'll always be valid.
      return true;
    })
    .def("__str__", ptzf::position_to_string)
    .def_property_readonly("gcode", ptzf::position_to_string);
#pragma GCC diagnostic pop

  py::class_<ptzf::Controller> controller(m, "Controller");
    // The gil must be released any IO to not hang python when using the
    // threading module. This way IO can be overlapped with Python work.
    // https://pybind11.readthedocs.io/en/stable/advanced/misc.html#global-interpreter-lock-gil
  controller.def(py::init<std::string, bool>(),
      py::arg("device"), py::arg("do_connect") = false,
      py::call_guard<py::gil_scoped_release>())
    .def(py::init<ptzf::Controller::Config>(),
      py::arg("config"),
      py::call_guard<py::gil_scoped_release>())
    .def("connect", &ptzf::Controller::connect, py::call_guard<py::gil_scoped_release>())
    .def("__enter__", [](ptzf::Controller& self){
      self.connect();
      return &self;
    })
    .def("__exit__", [](ptzf::Controller& self, py::object exc_type, py::object exc_value, py::object tb) {
      // we don't care what or if went wrong. We just call disconnect.
      (void)exc_type;
      (void)exc_value;
      (void)tb;
      // for the moment, disconnect always returns true. this might change in
      // the future.
      (void)self.disconnect();
    })
    .def("disconnect", &ptzf::Controller::disconnect, py::call_guard<py::gil_scoped_release>())
    .def("is_connected", &ptzf::Controller::is_connected)
    .def("go", &ptzf::Controller::go, py::call_guard<py::gil_scoped_release>())
    .def("go_home", &ptzf::Controller::go_home, py::call_guard<py::gil_scoped_release>());

  py::class_<ptzf::Controller::Config> config(controller, "Config");
  config.def_readonly("device", &ptzf::Controller::Config::device)
    .def(py::init<std::string, const ptzf::Position, const ptzf::Position, bool>(),
      py::arg("device") = std::string(ptzf::DEFAULT_DEVICE),
      py::arg("min") = ptzf::DEFAULT_MIN_POSITION,
      py::arg("max") = ptzf::DEFAULT_MAX_POSITION,
      py::arg("do_connect") = false)  // for python it's better to not connect 
      // on construct and always use `connect` disconnect or the context
      // manager.
    .def_property_readonly_static("DEFAULT_DEVICE", [](){ return ptzf::DEFAULT_DEVICE; })
    .def_property_readonly_static("DEFAULT_MIN_POSITION", [](){ return ptzf::DEFAULT_MIN_POSITION; })
    .def_property_readonly_static("DEFAULT_MAX_POSITION", [](){ return ptzf::DEFAULT_MAX_POSITION; })
    .def_property_readonly_static("DEFAULT_DO_CONNECT", [](){ return ptzf::DEFAULT_DO_CONNECT; })
    .def_readonly("min", &ptzf::Controller::Config::min)
    .def_readonly("max", &ptzf::Controller::Config::max)
    .def_readonly("do_connect", &ptzf::Controller::Config::do_connect)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def_static("from_json", ptzf::Controller::Config::from_json)
    .def("to_json", &ptzf::Controller::Config::to_json)
    .def("is_valid_position", &ptzf::Controller::Config::is_valid_position);

  m.attr("DEFAULT_DEVICE") = ptzf::DEFAULT_DEVICE;
  m.attr("DEFAULT_MIN_POSITION") = ptzf::DEFAULT_MIN_POSITION;
  m.attr("DEFAULT_MAX_POSITION") = ptzf::DEFAULT_MAX_POSITION;
  m.attr("DEFAULT_DO_CONNECT") = ptzf::DEFAULT_DO_CONNECT;
}

// clang-format on