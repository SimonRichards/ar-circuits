#include "StdAfx.h"
#include "Wire.h"
#include "ARScene.h"

using namespace std;

Wire::Wire(ARScene *cA, ARScene *cB, int lA, int lB) :
compA(cA), compB(cB), leadA(lA), leadB(lB) {
    osg::ref_ptr<osg::Cylinder> wireShape(new osg::Cylinder(osg::Vec3f(),1,2));
    wireShape->setRadius(20);
    osg::ref_ptr<osg::ShapeDrawable> wireDrawable(new osg::ShapeDrawable(wireShape));
    this->addDrawable(wireDrawable);
}

Wire::~Wire(){}

void Wire::update() {
    osg::Vec3d &start = compA->getCoord(leadA);
    osg::Vec3d &end   = compB->getCoord(leadB);
    auto c = dynamic_cast<osg::Cylinder*>(getDrawable(0)->getShape());
    c->setCenter((start+end)/2);
    c->setHeight((start - end).length());
    osg::Quat q;
    q.makeRotate(start, end);
    c->setRotation(q);
}

bool Wire::is(ARScene* s1, ARScene* s2, int l1, int l2) {
    return (compA == s1 && compB == s2 && leadA == l1 && leadB == l2) ||
           (compA == s2 && compB == s1 && leadA == l2 && leadB == l1);
}