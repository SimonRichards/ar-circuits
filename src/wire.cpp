#include "StdAfx.h"
#include "Wire.h"
#include "ARScene.h"

using namespace std;

Wire::Wire(ARScene *cA, ARScene *cB, int lA, int lB) :
compA(cA), compB(cB), leadA(lA), leadB(lB) {
	osg::ref_ptr<osg::Geode> wireGeode = new osg::Geode;
    wireShape = new osg::Cylinder(osg::Vec3f(),1,0);
    wireDrawable = new osg::ShapeDrawable(wireShape);
	wireDrawable->setColor(osg::Vec4(0.5,0.5,0.5,1));
    wireGeode->addDrawable(wireDrawable);
	this->addChild(wireGeode);

	osg::ref_ptr<osg::Geode> viGeode = new osg::Geode;

	viShape = new osg::Cylinder(osg::Vec3f(),1,0);
    viDrawable = new osg::ShapeDrawable(viShape);
	viDrawable->setColor(osg::Vec4(0.1,0.1,0.8,0.5));
    viGeode->addDrawable(viDrawable);

	this->addChild(viGeode);

	osg::ref_ptr<osg::StateSet> viStateSet = new osg::StateSet;
    viStateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
    viStateSet->setRenderBinDetails( 1, "RenderBin");
	viGeode->setStateSet(viStateSet);
}

Wire::~Wire() {
    //removeDrawables(getNumDrawables());
}

void Wire::update(osg::Vec3d normal, float viHeight, float viRadius) {
    osg::Vec3d &start = compA->getCoord(leadA);
    osg::Vec3d &other = compA->getCoord(leadB);
    osg::Vec3d &end   = compB->getCoord(leadB);
    osg::Quat q;
    q.makeRotate(osg::Vec3d(0, 0, 1), end - start);
	
    wireShape->setCenter((start+end)/2);
    wireShape->setHeight((start - end).length());
	wireShape->setRotation(q);
    wireDrawable->dirtyDisplayList();
	wireDrawable->dirtyBound();
	
    viShape->setCenter((start+end)/2);
    viShape->setHeight((start - end).length());
	viShape->setRotation(q);
	viShape->setCenter(viShape->getCenter() + normal*viHeight);
	viShape->setRadius(viRadius);
    viDrawable->dirtyDisplayList();
	viDrawable->dirtyBound();
}

bool Wire::is(ARScene* s1, ARScene* s2, int l1, int l2) {
    return (compA == s1 && compB == s2 && leadA == l1 && leadB == l2) ||
           (compA == s2 && compB == s1 && leadA == l2 && leadB == l1);
}