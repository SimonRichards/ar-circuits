#pragma once

#include "StdAfx.h"
#include "gnucap-lib.h"

class ARScene;

class Wire : public osg::Group
{
private:
    osg::ref_ptr<osg::Cylinder> wireShape;
    osg::ref_ptr<osg::ShapeDrawable> wireDrawable;
    osg::ref_ptr<osg::Cylinder> viShape;
    osg::ref_ptr<osg::ShapeDrawable> viDrawable;
public:
	Wire(ARScene *cA, ARScene *cB, int lA, int lB);
	~Wire();
	ARScene *compA, *compB;
	int leadA, leadB;
	void update(osg::Vec3d normal, float viHeight = 0.0);

    bool is(ARScene* s1, ARScene* s2, int l1, int l2);
};
