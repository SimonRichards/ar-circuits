#include "stdafx.h"
#include "components.h"
#include "simulator.h"
#include <iostream>
#define PI 3.14159265

using namespace std;

namespace circuit_sim {

	ACSource::ACSource(double v, double b, double f, Simulator* sim){
		voltage = v;
		bias = b;
		frequency = f;
	}

	ACSource::~ACSource(){
	}

	void ACSource::step() {
		double w = 2 * PI * (_sim->t) * frequency;
		_sim->updateVoltageSource(nodes[0], nodes[1], voltSource, sin(w) * voltage + bias);

	}
	void ACSource::calculateCurrent() {

	}
	void ACSource::stamp() {
		_sim->stampVoltageSource(nodes[0], nodes[1], voltSource);

	}
	void ACSource::startIteration() {

	}

	void ACSource::printState() {

	}

	double ACSource::getVoltageDiff() {
		return volts[1] - volts[0];
	}

	DCSource::DCSource(double v, Simulator* sim){
		_sim = sim;
		voltage = v;
	}

	DCSource::~DCSource(){
	}

	double DCSource::getVoltageDiff() {
		return volts[1] - volts[0];
	}

	void DCSource::step() {}

	void DCSource::calculateCurrent() {}

	void DCSource::printState() {
		cout << "DC Source at " << voltage << "V" << endl;
	}

	void DCSource::stamp() {
		_sim->stampVoltageSource(nodes[0], nodes[1], voltSource, voltage);
	}

	void DCSource::startIteration() {}
}