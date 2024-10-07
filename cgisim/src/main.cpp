#include <iostream>
#include "pybind11/pybind11.h"
#include "Listener.h"
#include "Listeners.h"
#include <string>
using namespace std;
void do_nothing(){}

int add(int a, int b){
    return a+b;
}

void callFactory(const string& name){
    cgisim::Factory<cgisim::Listener>::produce(name)->getName();
}

PYBIND11_MODULE(cgisim, m){
    m.doc() = "Cat's Tail Tavern Simluator";
    m.def("add",&add, "Add function for bind test");
    m.def("callfac", &callFactory, "Listener Factory");
}