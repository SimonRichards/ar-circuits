#pragma once
#include "Logic.hpp"
using namespace std;

class Or : public Gate {
public:
	virtual bool determine();
};
