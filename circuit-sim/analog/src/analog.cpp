#include "stdafx.h"
#include "components.h"
#include "simulator.h"
#include <iostream>
#include <Windows.h>

using namespace circuit_sim;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Simulator sim;
	sim.setupTest();
	volatile unsigned long j = GetTickCount();
	for (int i = 0; i < 10000; i++) {
		//cout << "Running step " << i << endl;
		sim.updateCircuit();
		//while (GetTickCount() - j < 20);
		//j = GetTickCount();
	}
		sim.printTestState();
		cout << endl;
	cout << "Press enter to continue." << endl;
	cin.get();
	return 0;
}

