#pragma once
#include "StdAfx.h"
using namespace OPIRALibrary;

osg::Node* createLights(osg::Node* scene);
osg::Node* findNamedNode(const std::string& searchName, osg::Node* currNode);


class modelOSG: public osg::Group {
public:
	osg::ref_ptr<osg::Node> model;
	osg::ref_ptr<osg::MatrixTransform> transform;
	osg::ref_ptr<osg::MatrixTransform> osgTransform;
	osg::ref_ptr<osg::Switch> visible;
	modelOSG(osg::Node *n);
};