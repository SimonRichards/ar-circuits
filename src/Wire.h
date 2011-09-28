#pragma once

#include "StdAfx.h"
#include "gnucap-lib.h"

class Wire
{
private:

public:
	Wire();
	~Wire();
	gnucap_lib::Component *compA, *compB;
	int leadA, leadB;
	void render();
	//void setEndPoints(coords);
};
