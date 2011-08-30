#include "StdAfx.h"
#include "Logic.h"

bool Sink::determine() {
    return _input->determine();
}

void Sink::connect(Gate* input) {
    _input = input;
}
