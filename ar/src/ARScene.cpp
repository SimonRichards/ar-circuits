#include "StdAfx.h"
#include "ARScene.h"

ARScene::ARScene(osg::Node* n, std::string markerPath, OPIRALibrary::Registration* r)
{
	init(n, markerPath, r);
}

ARScene::ARScene(std::string modelPath, std::string markerPath, OPIRALibrary::Registration* r)
{
	init(osgDB::readNodeFile(modelPath), markerPath, r);
}


void ARScene::init(osg::Node* n, std::string markerPath, OPIRALibrary::Registration* r){
	model = n;
	markerTransform = new osg::MatrixTransform(osg::Matrix::identity());
	sceneTransform = new osg::MatrixTransform(osg::Matrix::identity());//osg::Matrix::rotate(osg::DegreesToRadians(180.0f), osg::X_AXIS));
	visible = new osg::Switch();
	visible->addChild(markerTransform); markerTransform->addChild(sceneTransform); sceneTransform->addChild(model); //transform->addChild(createLights(this));
	this->addChild(visible);
	r->addMarker(markerPath);
	markerID = markerPath;
}

void ARScene::setTransform(osg::Matrix transform){
	this->markerTransform->setMatrix(transform);
}

void ARScene::setSceneTransformCallback(osg::NodeCallback* cb){
	sceneTransform->setUpdateCallback(cb);
}

void ARScene::setVisibility(bool vis){
	if(vis){
		visible->setAllChildrenOn();
	}
	else{
		visible->setAllChildrenOff();
	}
}

string ARScene::getMarkerID(){
	return markerID;
}

osg::Node* ARScene::getSceneNode(){
	return model;
}

ARScene::~ARScene(void)
{
}
