#include "StdAfx.h"

#include "Page.h"


Page::Page(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
{
	for(int i = 0; i < config["scenes"].getLength(); i++){
		scenes.push_back(new ARScene(config["scenes"][i], r));
	}
}

vector<ARScene*> Page::getScenes(){
	return scenes;
}

void Page::updatePage(){
	for each (ARScene* scene in scenes){
		scene->setVisibility(scene->isMarkerVisible());
		scene->updateTextures();
		scene->setOriginMatrix(scene->getMarkerMatrix());
	}
}

Page::~Page(void)
{
}
