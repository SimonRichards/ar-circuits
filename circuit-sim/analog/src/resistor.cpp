#include "stdafx.h"
#include "components.h"
#include "simulator.h"

namespace circuit_sim {

	Resistor::Resistor(double r, Simulator* sim) {
		resistance = r;
		_sim = sim;
		volts = new double[2];
		nodes = new int[2];
	}
	
	Resistor::~Resistor() {

	}

	void Resistor::calculateCurrent() {
		current = (volts[0] - volts[1]) / resistance;

	}
	void Resistor::stamp() {
		_sim->stampResistor(nodes[0], nodes[1], resistance);
	}
}