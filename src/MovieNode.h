#pragma once

#include "StdAfx.h"

class MovieNode
{
public:
	MovieNode(osg::ref_ptr<osg::Node> model, vector<osg::ref_ptr<osg::Image*>> frames);
	MovieNode(osg::ref_ptr<osg::Node> model, vector<string> imgPaths, bool playing);
	MovieNode(const libconfig::Setting &config);
	~MovieNode(void);
	void nextFrame();
	bool isPlaying();

private:
	osg::ref_ptr<osg::Node> model;
	osg::ref_ptr<osg::Texture2D> frameTexture;
	vector<osg::ref_ptr<osg::Image>> frames;
	int currentFrame;
	int numFrames;
	bool playing;
};

