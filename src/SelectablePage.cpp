#include "StdAfx.h"
/*
#include "ARCircuits.h"


SelectableARCircuit::SelectableARCircuit(const libconfig::Setting& config, OPIRALibrary::RegistrationARToolkit* r)
	: ARCircuit(config, r), lastMissingScene(scenes.at(0))
{
}

void SelectableARCircuit::updateARCircuit(){
	for each (ARScene* scene in scenes){
		if(scene->isMarkerVisible()){
			if(scene == lastMissingScene){
				scene->setVisibility(true);
			}
			else{
				scene->setVisibility(false);
			}
			scene->updateTextures();
			scene->setOriginMatrix(scene->getMarkerMatrix());
		}
		else{
			scene->setVisibility(false);
			lastMissingScene = scene;
		}
	}
}*/