#include <iostream>
#include "pybind11/pybind11.h"
void do_nothing(){}

int add(int a, int b){
    return a+b;
}

PYBIND11_MODULE(cgisim, m){
    m.doc() = "Cat's Tail Tavern Simluator";
    m.def("add",&add, "Add function for bind test");
}