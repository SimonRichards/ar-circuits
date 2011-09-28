#pragma once

#include "StdAfx.h"
#include "gnucap-lib.h"

class ARScene;

class Wire : public osg::Group
{
private:

public:
	Wire(ARScene *cA, ARScene *cB, int lA, int lB) :
	  compA(cA), compB(cB), leadA(lA), leadB(lB) {}
	~Wire();
	ARScene *compA, *compB;
	int leadA, leadB;
	void render();
};
