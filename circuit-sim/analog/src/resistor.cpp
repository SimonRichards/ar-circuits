#include "stdafx.h"
#include "components.h"
#include "simulator.h"
#include <iostream>

namespace circuit_sim {

	double Component::currentMult;
	double Component::powerMult;

	Resistor::Resistor(double r, Simulator* sim) {
		resistance = r;
		_sim = sim;
	}
	
	Resistor::~Resistor() {
	}

	void Resistor::calculateCurrent() {
		double voltLeft = volts[0];
		double voltRight= volts[1];
		current = (volts[0] - volts[1]) / resistance;
	}
	void Resistor::stamp() {
		_sim->stampResistor(nodes[0], nodes[1], resistance);
	}

	void Resistor::printState() {
		cout << "Resistance " << resistance << ", voltage " << getVoltageDiff() << " and power " << getPower() << endl;
	}
}