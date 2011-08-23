#include "Logic.hpp"
using namespace std;

void Gate::scrub() {
    _inputs.clear();
}

void Gate::connect(Gate gate) {
    _inputs.push_back(gate);
}

bool Gate::determine() {
	return false; // why do I need this
}