#include "StdAfx.h"
#include "Wire.h"
#include "ARScene.h"

using namespace std;

Wire::~Wire(){}

void Wire::render() {
	osg::Vec3d &start = compA->getCoord(leadA);
	osg::Vec3d &end   = compB->getCoord(leadB);

	//this->
}