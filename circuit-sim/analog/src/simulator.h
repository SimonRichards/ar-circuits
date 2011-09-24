#pragma once
#include "stdafx.h"
#include "components.h"
#include "rowinfo.h"
#include "node.h"

using namespace std;

namespace circuit_sim {

	class Simulator {
	private:
		const static int sourceRadius = 7;
		bool analyseFlag;
		bool dumpMatrix;
		static const int pause = 10;
		vector<Component*> components;
		double** circuitMatrix;
		double* circuitRightSide;
		double* origRightSide;
		double** origMatrix;
		RowInfo *circuitRowInfo;
		int* circuitPermute;
		bool circuitNonLinear;
		int voltageSourceCount;
		int circuitMatrixSize, circuitMatrixFullSize;
		bool circuitNeedsMap;
		int circuitBottom;
		bool converged;
		int subIterations;
		const static int resct = 6;
		long lastTime, lastFrameTime, lastIterTime, secTime;
		int frames, steps, framerate, steprate;
		vector<Node> nodes;
		Component** voltageSources;
		
		Resistor *testResistor;
		DCSource *testSource;
		Capacitor *testCapacitor;
		Node *leftNode, *centreNode, *rightNode;
		NodeLink *resistorLeftLink, *resistorRightLink,
		     *sourceLeftLink, *sourceRightLink,
		     *capLeftLink, *capRightLink;
	public:
		Simulator();
		~Simulator();
		void setupTest();
		void updateCircuit();
		void printTestState();
		double t;
		double timeStep;
		void analyseCircuit();
		bool lu_factor(double** a, int n, int* ipvt);
		void lu_solve(double** a, int n, int* ipvt, double* b);
		void runCircuit();
		void stampVCVS(int n1, int n2, double coef, int vs);
		void stampVoltageSource(int n1, int n2, int vs, double v);
		void stampVoltageSource(int n1, int n2, int vs);
		void updateVoltageSource(int n1, int n2, int vs, double v);
		void stampResistor(int n1, int n2, double r);
		void stampConductance(int n1, int n2, double r0);
		void stampVCCurrentSource(int cn1, int cn2, int vn1, int vn2, double g);
		void stampCurrentSource(int n1, int n2, double i);
		void stampRightSide(int i, double x);
		void stampRightSide(int i);
		void stampNonLinear(int i);
		void stampMatrix(int i, int j, double x);
		void stampCCCS(int n1, int n2, int vs, double gain);
		double getIterCount();
	};

	class FindPathInfo {
	public:
		bool *used;
		int dest;
		Component* firstElm;
		int type;
		static const int INDUCT  = 1;
		static const int VOLTAGE = 2;
		static const int SHORT   = 3;
		static const int CAP_V   = 4;
		FindPathInfo(int t, Component* c, int d, int numNodes);
		~FindPathInfo();
		bool findPath(int n1, vector<Component*> components) { return findPath(n1, -1, components); }
		bool findPath(int n1, int depth, vector<Component*> components);
	};

}