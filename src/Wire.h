#pragma once

#include "StdAfx.h"
#include "gnucap-lib.h"

class ARScene;

class Wire : public osg::Geode
{
private:

public:
	Wire(ARScene *cA, ARScene *cB, int lA, int lB);
	~Wire();
	ARScene *compA, *compB;
	int leadA, leadB;
	void update();

    bool is(ARScene* s1, ARScene* s2, int l1, int l2);
};
