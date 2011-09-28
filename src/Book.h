#pragma once

#include "StdAfx.h"

#include "ARCircuit.h"
#include "ARToolkit2.7OpenCV.h"

using namespace OPIRALibrary;

class Book
{
public:
	Book(const libconfig::Setting& config, int argc, char** argv);
	~Book(void);

	vector<ARScene*> getScenes(int ARCircuitNum);
	void run();

private:
	void goToAndPlay(float time);
	void initOGL(int argc, char **argv);
	void render(IplImage* frame_input, std::vector<MarkerTransform> mt);

	int camWidth, camHeight;
	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Image> mVideoImage;
	osg::ref_ptr<osg::Camera> fgCamera;
	vector<ARCircuit*> ARCircuits;
	Capture* camera;
	RegistrationARToolkit* reg;
};

