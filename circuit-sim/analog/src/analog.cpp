#include "stdafx.h"
#include "components.h"
#include "simulator.h"
#include <iostream>

using namespace circuit_sim;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Simulator sim;
	sim.setupTest();
	for (int i = 0; i < 10; i++) {
		cout << "Running step " << i << endl;
		sim.updateCircuit();
		sim.printTestState();
		cout << endl;
	}
	cout << "Press enter to continue." << endl;
	cin.get();
	return 0;
}

