#include "Logic.hpp"
using namespace std;

bool Sink::determine() {
    return _input.determine();
}

void Sink::connect(Gate input) {
    _input = input;
}
