#include "StdAfx.h"

#include "MovieNode.h"

MovieNode::MovieNode(osg::ref_ptr<osg::Node> model, vector<string> imgFiles, bool playing) //TODO: add check on model node: crash caused by incorrect movieNode name in cfg
	: model(model), currentFrame(0), playing(playing),
	frameTexture(dynamic_cast<osg::Texture2D*>(model->getStateSet()->getTextureAttribute(0, osg::StateAttribute::TEXTURE)))
{
	for each (string path in imgFiles){
		// load an image by reading a file: 
		osg::ref_ptr<osg::Image> frameImage = osgDB::readImageFile(modelDir + path);
		if (!frameImage)
		{
			std::cout << "Unable to read texture: " << path << std::endl;
			break;
		}
		// Assign the texture to the image we read from file: 
		frames.push_back(frameImage);
	}
}

void MovieNode::nextFrame(){
	if(frames.size()){
		currentFrame = currentFrame >= frames.size() - 1 ? 0 : currentFrame + 1;
		frameTexture->setImage(frames.at(currentFrame));
	}
}

bool MovieNode::isPlaying(){
	return playing;
}
MovieNode::~MovieNode(void)
{
}
