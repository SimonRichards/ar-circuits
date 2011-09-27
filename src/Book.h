#pragma once

#include "StdAfx.h"

#include "Page.h"
#include "Pages.h"
#include "ARToolkit2.7OpenCV.h"

using namespace OPIRALibrary;

class Book
{
public:
	Book(const libconfig::Setting& config, int argc, char** argv);
	~Book(void);

	vector<ARScene*> getScenes(int pageNum);
	void run();

private:
	void goToAndPlay(float time);
	void initOGL(int argc, char **argv);
	void render(IplImage* frame_input, std::vector<MarkerTransform> mt);

	int camWidth, camHeight;
	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Image> mVideoImage;
	osg::ref_ptr<osg::Camera> fgCamera;
	vector<Page*> pages;
	Capture* camera;
	RegistrationARToolkit* reg;
};

