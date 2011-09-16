#pragma once
#include "stdafx.h"
//#include "simulator.h"

namespace circuit_sim {

	class Component {
	private:
		const static int voltageRange = 5;
		const static int colorScaleCount = 32;
		static double currentMult, powerMult;
		//static Simulator sim;

		int *nodes;
		int voltSource;
		int dx, dy, dsign;
		double dn, dpx1, dpy1;
		double *volts;
		double current, curcount;

	public:
		void setCurrent(double c) { current = c;}
		virtual void step() = 0;
		virtual void calculateCurrent() = 0;
		virtual void stamp() = 0;
		virtual void startIteration() = 0;
		void setNodeVoltage(int n, double c) {
			volts[n] = c;
			calculateCurrent();
		}
	};
	
	class Source : public Component {
	private:
		double voltage;
	public:
		virtual void step();
		virtual void calculateCurrent();
		virtual void stamp();
		virtual void startIteration();
	};
	
	class Resistor : public Component {
	private:
		double resistance;
	public:
		virtual void step();
		virtual void calculateCurrent();
		virtual void stamp();
		virtual void startIteration();
	};
}