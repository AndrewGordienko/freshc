#include <pybind11/pybind11.h>

int addNumbers(int a, int b);

PYBIND11_MODULE(example, m) {
    m.def("add_numbers", &addNumbers, "Add two numbers");
}
