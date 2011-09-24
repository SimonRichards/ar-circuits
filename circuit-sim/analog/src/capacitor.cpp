#include "stdafx.h"
#include "components.h"
#include "simulator.h"
#include <iostream>

namespace circuit_sim {
	Capacitor::Capacitor(double c, Simulator* sim) {
		_sim = sim;
		capacitance = c;
		reset();
	}

	Capacitor::~Capacitor() {}

	void Capacitor::reset() {
		current = 0;
		curcount = 0;
		voltdiff = 1e-3;
	}

	void Capacitor::calculateCurrent() {
	    double voltdiff = volts[0] - volts[1];
	    if (compResistance > 0)
			current = voltdiff/compResistance + curSourceValue;
	}

	void Capacitor::setNodeVoltage(int n, double c) {
		volts[n] = c;
		calculateCurrent();
		voltdiff = volts[0]-volts[1];
	}

	void Capacitor::startIteration() {
		curSourceValue = -voltdiff/compResistance-current;
	}

	void Capacitor::stamp() {
		compResistance = _sim->timeStep/(2*capacitance);
	    _sim->stampResistor(nodes[0], nodes[1], compResistance);
	    _sim->stampRightSide(nodes[0]);
	    _sim->stampRightSide(nodes[1]);
	}

	void Capacitor::step() {
		_sim->stampCurrentSource(nodes[0], nodes[1], curSourceValue);
	}

	void Capacitor::printState() {
		cout << "Capacitor has voltage " << volts[1] - volts[0] << endl;
	}
}