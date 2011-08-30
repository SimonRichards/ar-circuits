#include "StdAfx.h"
#include "Logic.h"

void Gate::scrub() {
    _inputs.clear();
}

void Gate::connect(Gate* gate) {
    _inputs.push_back(gate);
}
