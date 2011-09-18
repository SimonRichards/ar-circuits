#pragma once
#include "stdafx.h"
//#include "simulator.h"

namespace circuit_sim {

	typedef enum {CAPACITOR, INDUCTOR, WIRE, SOURCE, CURRENT_SOURCE} component_type_t;

	class Simulator;

	class Component {
	private:
		const static int voltageRange = 5;
		const static int colorScaleCount = 32;
		static double currentMult, powerMult;

		int dx, dy, dsign;
		double dn, dpx1, dpy1;

	protected:
		Simulator* _sim;
		int *nodes;
		double *volts;
		double current, curcount;
		int voltSource;

	public:
		virtual int getPostCount() { return 2; }
		void setCurrent(double c) { current = c;}
		double getCurrent() { return current; }
		virtual void step() {};
		virtual void calculateCurrent() = 0;
		virtual void stamp() = 0;
		virtual void startIteration() {};
		virtual component_type_t getType() = 0;
		void setNodeVoltage(int n, double c) {
			volts[n] = c;
			calculateCurrent();
		}
		virtual void reset() {
			volts[0] = 0;
			volts[1] = 0;
			curcount = 0;
		}
		virtual bool getConnection(int n1, int n2) { return true;}
		int getNode(int n) { return nodes[n]; }
		virtual int getVoltageSourceCount() {return 0;}
		void setVoltageSource(int v) { voltSource = v; }
		void setNode(int p, int n) { nodes[p] = n; }
	};
	
	class DCSource : public Component {
	private:
		double voltage;
	public:
		DCSource(double v, Simulator* sim);
		~DCSource();
		virtual void step();
		virtual void calculateCurrent();
		virtual void stamp();
		virtual void startIteration();
		virtual int getVoltageSourceCount() {return 1;}
	};

	class ACSource : public Component {
	private:
		double voltage;
		double frequency;
		double bias;
	public:
		ACSource(double v, double b, double f, Simulator* sim);
		~ACSource();
		virtual void step();
		virtual void calculateCurrent();
		virtual void stamp();
		virtual void startIteration();
		virtual int getVoltageSourceCount() {return 1;}
	};	
	
	class Resistor : public Component {
	private:
		double resistance;
	public:
		Resistor(double r, Simulator* sim);
		~Resistor();
		virtual void calculateCurrent();
		virtual void stamp();
	};
}