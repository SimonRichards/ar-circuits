#pragma once

#include "StdAfx.h"
#include "gnucap-lib.h"

class ARScene;

class Wire
{
private:

public:
	Wire();
	~Wire();
	ARScene *compA;
	ARScene *compB;
	int leadA, leadB;
	void render();
	//void setEndPoints(coords);
};
