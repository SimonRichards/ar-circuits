#pragma once

#include <OPIRALibrary.h>
#include <ARToolkit2.7OpenCV.h>

//OpenSceneGraph
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Texture2D>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/io_utils>
#include <GL/glut.h>

#include <iostream>
#include <string>

class ARScene :
	public osg::Group
{
public:
	ARScene(osg::Node* n, std::string markerPath, OPIRALibrary::Registration* r);

	ARScene(std::string modelPath, std::string markerPath, OPIRALibrary::Registration* r);

	void setVisibility(bool visible);
	
	void setTransform(osg::Matrix transform);

	void setSceneTransformCallback(osg::NodeCallback* cb);

	osg::Node* getSceneNode();

	string getMarkerID();

	~ARScene(void);

private:
	osg::ref_ptr<osg::Node> model;
	osg::ref_ptr<osg::MatrixTransform> markerTransform;
	osg::ref_ptr<osg::MatrixTransform> sceneTransform;
	osg::ref_ptr<osg::Switch> visible;
	string markerID;

	void init(osg::Node* n, std::string markerPath, OPIRALibrary::Registration* r);
};