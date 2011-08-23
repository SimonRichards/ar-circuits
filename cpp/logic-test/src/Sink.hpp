#pragma once
#include "Logic.hpp"
using namespace std;

class Sink {
private:
	Gate _input;

public:
	bool determine();
	void connect(Gate input);
};
