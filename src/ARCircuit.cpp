#include "StdAfx.h"

#include "ARCircuit.h"


ARCircuit::ARCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
{
	for(int i = 0; i < config["scenes"].getLength(); i++){
		scenes.push_back(new ARScene(config["scenes"][i], r));
	}
}

vector<ARScene*> ARCircuit::getScenes(){
	return scenes;
}

void ARCircuit::updateARCircuit(){
	for each (ARScene* scene in scenes){
		scene->setVisibility(scene->isMarkerVisible());
		scene->updateTextures();
		scene->setOriginMatrix(scene->getMarkerMatrix());
	}
}

ARCircuit::~ARCircuit(void)
{
}
