#include "StdAfx.h"
#include "Wire.h"
#include "ARScene.h"

using namespace std;

Wire::Wire(ARScene *cA, ARScene *cB, int lA, int lB) :
compA(cA), compB(cB), leadA(lA), leadB(lB) {
    wireShape = new osg::Cylinder(osg::Vec3f(),1,0);
    wireDrawable = new osg::ShapeDrawable(wireShape);
	wireDrawable->setColor(osg::Vec4(0.5,0.5,0.5,1));
    this->addDrawable(wireDrawable);
}

Wire::~Wire() {
    removeDrawables(getNumDrawables());
}

void Wire::update() {
    osg::Vec3d &start = compA->getCoord(leadA);
    osg::Vec3d &end   = compB->getCoord(leadB);
    wireShape->setCenter((start+end)/2);
    wireShape->setHeight((start - end).length());
    osg::Quat q;
    q.makeRotate(osg::Vec3d(0, 0, 1), end - start);
    //cout << q << endl;
    wireDrawable->dirtyDisplayList();
}

bool Wire::is(ARScene* s1, ARScene* s2, int l1, int l2) {
    return (compA == s1 && compB == s2 && leadA == l1 && leadB == l2) ||
           (compA == s2 && compB == s1 && leadA == l2 && leadB == l1);
}